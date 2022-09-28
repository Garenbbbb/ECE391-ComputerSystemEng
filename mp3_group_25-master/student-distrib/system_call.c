#include "system_call.h"

#define USER_IMAGE 0x8048000
#define SHIFT24 24
#define SHIFT16 16
#define SHIFT8  8
#define PCB_MASK 0xFFFFE000
#define EIGHT_MB 0x800000
#define EIGHT_KB 0x2000
#define FOURTY_KB 0xA000


/* array of the six processes. It is set to 1 if the corresponding 
   process exists */
int process_arr[6] = {0, 0, 0, 0, 0, 0};
// correct value for magic number at the beginning of the executable file
char elf_header[4] = {0x7f, 0x45, 0x4c, 0x46};


/*
 *   halt
 *   DESCRIPTION: end a running program
 *   INPUTS: status number
 *   OUTPUTS: status number
 *   RETURN VALUE: status number
 */
int32_t halt (uint8_t status){
    return real_halt((uint32_t) status);
}

/*
 *   halt helper
 *   DESCRIPTION: end a running program
 *   INPUTS: status number
 *   OUTPUTS: status number
 *   RETURN VALUE: status number
 */
int32_t real_halt (uint32_t status){
    // get return value
    uint32_t ret_value = status;

    // get pcb
    pcb_t* pcb = get_cur_pcb();
    
    // get current process if
    int cur_pid = pcb->pid;

    // update the process array
    process_arr[pcb->pid] = 0;

    // clear the fd
    // clear the entries of stdin and stdout
    pcb->fdt[0].flags = 0;
    pcb->fdt[1].flags = 0;
    // loop though the remianing files to close them
    uint32_t fd_index;   // loop index
    for (fd_index = 2; fd_index < 8; fd_index++) {
      if ( pcb->fdt[fd_index].flags != 0) {
        close(fd_index);
      }
    }

    // clear the argument array
    int i;  // loop index
    for (i = 0; i < 128; i++) {
      pcb->argument[i] = '\0';
    }

    // If this is the first process then we cannnot halt to the parent process
    if (cur_pid <= 2) {
      // execute shell program again in theis process
      printf("Quiting last shell is prohibited by Xushen\n");
      execute((uint8_t*)"shell");
    }
    
    // update current pid
    cur_pid = pcb->parent->pid;
 
    // restore parent's paging
    paging_set_up(cur_pid);

    // set tss
    tss.ss0 = KERNEL_DS;
    tss.esp0 = EIGHT_MB - (cur_pid * EIGHT_KB) - 4;

    // restore parent's data and return
    asm volatile( "xorl %%eax, %%eax;"
                  "movl %0, %%eax;"    
                  "movl %1, %%esp;"
                  "movl %2, %%ebp;"
                  "jmp halt_return"
                  : // no outputs
                  :"d" (ret_value), "b" (pcb->parent->esp), "c" (pcb->parent->ebp) 
                );
                
    return 0;
}

/*
 *   execute (const uint8_t* command)
 *   DESCRIPTION: start a new program
 *   INPUTS: command argument
 *   OUTPUTS: none
 *   RETURN VALUE: None
 */
int32_t execute (const uint8_t* command){

  pcb_t* pcb;    // PCB ptr
  int cur_pid;   // process id
  int i, j;      // loop index

  // loop through the process array to check whether there is space
  // to start a new process
  for (i = 0; i < 6; i++) {
    if (process_arr[i] != 1) {
      break;
    }
  }
  // If i is 6, we cannot start another new process
  if (i == 6) {
    printf("Hushen: There are already 6 processes running, cannot open new process\n");
    return 0;
  }
  // Else the current pid is the value of i
  else {
    cur_pid = i;
  }

  if (command == NULL) { //check the validity of buffer
    return -1;
  }

  // parse argument
  int name_len = 0;
  char exe_command[32];   //max length of file name is 32
  // initialize exe_command to \0 
  for(i = 0;i < 32;i ++){
      exe_command[i] = 0;
  }
  // loop through the command buffer to find the correct file name
  int flag1 = 0;   // flag that indicating whether there is argument;
  for(i = 0;i < 128;i ++){
      if(command[i] == ' '  && name_len == 0){  // ignore all ' ' in front of the filename
          continue;
      }
      else if(command[i] == '\0' && name_len != 0){
          break;
      }
      else if(command[i] != ' ' && name_len < 32){   // record the filename
          exe_command[name_len] = command[i];
          name_len ++;
      }
      // ignore contents after the first ' ' after the first filename
      else if(command[i] == ' ' && name_len != 0){   
          flag1 = 1;
          break;                                     
      }
  }

  // create PCB for the new process
  // calculate the position of the PCB in kernel space
  // 0x800000 is the addr of 8MB, 0x2000 is the addr of 8KB
  pcb = (pcb_t *)(EIGHT_MB - (cur_pid+1) * EIGHT_KB);
  
  // store the argument in the PCB
  if (flag1 == 0) {
    pcb->argument[0] = '\0';
  }
  else {
    while (command[i] == ' ') {
      i += 1;
    }
    int arg_index = 0;
    while (command[i] != '\0') {
      pcb->argument[arg_index] = command[i];
      arg_index += 1;
      i += 1;
    }
    pcb->argument[arg_index] = '\0';
  }

  // check executable
  // load header data of the executable file
  uint8_t header_buf[40];   // size of the header data is 40 bytes
  dentry_t cur_dentry;
  j = read_dentry_by_name((uint8_t *)exe_command, &cur_dentry);
  // check whether the filename is valid
  if(j == -1){return -1;}    
  read_data(cur_dentry.inode_index, 0, header_buf, 40);

  int8_t check_buf[4];  // holds the magic number 

  // fill the check buffer and the entry point buffer
  for (j = 0; j < 4; j++) {
    check_buf[j] = header_buf[j];
  }
  
  // check whether it is executable by comparing magic numbers
  if(0 != strncmp(check_buf, elf_header, 4)){return -1;}

  // it is executable, so update the process array
  process_arr[cur_pid] = 1;

  // set up program paging and load user-level program
  paging_set_up(cur_pid);

  //copy file to the physical memory
  read_data(cur_dentry.inode_index, 0, (void *)USER_IMAGE, FOURTY_KB);  
 
  // find the entry point 
  uint32_t entry_point_start = USER_IMAGE + 24;  // 24 is the starting byte of the entry point
  uint32_t entry_point =0;
  // add from 3 to 0 cause we have 4 bytes in total
  entry_point = *((uint8_t *)entry_point_start + 3) << SHIFT24; 
  entry_point |= *((uint8_t*)entry_point_start + 2) << SHIFT16; 
  entry_point |= *((uint8_t*)entry_point_start + 1) << SHIFT8;  
  entry_point |= *((uint8_t*)entry_point_start + 0);
  
  // initiate PCB
  pcb->pid = cur_pid;
  if (cur_pid == 0 || cur_pid == 1 || cur_pid == 2)   pcb->parent = NULL;
  else {
    // find parent's pcb
    pcb->parent = (pcb_t *)(EIGHT_MB - (schedule_arr[cur_term_idx] + 1) * EIGHT_KB);  

    // store parent data
    // maintain parent esp and ebp in the parent PCB
    asm volatile( "movl %%esp, %0;"
                  "movl %%ebp, %1;"
                  :"=r" (pcb->parent->esp), "=r" (pcb->parent->ebp) 
                  : /* no inputs */
                );
  }
  schedule_arr[cur_term_idx] = i;
  term[i].pcb_idx = i;

  set_up_io_file(pcb);  // set up stdin and stdout when create fdt in pcb

  // set tss
  tss.ss0 = KERNEL_DS;
  tss.esp0 = EIGHT_MB - (cur_pid * EIGHT_KB) - 4;  // -4 to save space for esp
  
  // get ss and cs values
  int user_ds = USER_DS;
  int user_cs = USER_CS;

  // iret to userspace 
  // set up the stack frame iret expects
  asm volatile( "pushl %0;"
                "pushl $0x83FFFFC;"  // this is the value of 132MB - 4B, bottom of user program paging  
                "pushfl;"
                "pushl %1;"
                "pushl %2;"
                "iret;"
                ".globl halt_return;"
                "halt_return: ;"    // label that halt program will jump back
                " leave;"
                " ret;"
                : // no output
                :"r" (user_ds), "r" (user_cs), "r" (entry_point)   // input
              );
  
  return 0;

}



/*
 *   read (int32_t fd, void* buf, int32_t nbytes)
 *   DESCRIPTION: read specific file in fd
 *   INPUTS: int32_t fd, void* buf, int32_t nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: num of  bytes read  -1 = failure
 */
int32_t read (int32_t fd, void* buf, int32_t nbytes){

  // get access to the current pcb
  pcb_t* pcb = get_cur_pcb();

  if (fd < 0 || fd >= 8 ) {
    return -1; // invalid fd
  }
  if (pcb->fdt[fd].flags == 0) {
    return -1; //  resourse is not avaliable
  }
  if (pcb->fdt[fd].file_operation_table_ptr == NULL) {
    return -1; // function doesn't exist
  }

  //read hwo many bytes
  int bytes_read =pcb->fdt[fd].file_operation_table_ptr->read(fd, buf, nbytes);
  
  return bytes_read;
}



/*
 *  write (int32_t fd, const void* buf, int32_t nbytes)
 *   DESCRIPTION:  write data to specif file
 *   INPUTS: int32_t fd, const void* buf, int32_t nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: how many bytes we write  -1 = failure
 */
int32_t write (int32_t fd, const void* buf, int32_t nbytes){

  // get access to the current pcb
  pcb_t* pcb = get_cur_pcb();

  if (fd < 0 || fd >= 8 ) {
    return -1; // invalid fd
  }
  if (pcb->fdt[fd].flags == 0) {
    return -1; //  resourse is not avaliable
  }
  if (pcb->fdt[fd].file_operation_table_ptr == NULL) {
    return -1; // function doesn't exist
  }
  //
  return pcb->fdt[fd].file_operation_table_ptr->write(fd, buf, nbytes);
}


/*
 *   open (const uint8_t* filename)
 *   DESCRIPTION: flush tlb by assign new page table
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: -1 fail 0  success
 */
int32_t open (const uint8_t* filename){
      // get access to the current pcb
      pcb_t* pcb = get_cur_pcb();

      int index_for_fdt;
      dentry_t cur_dentry;
      if (read_dentry_by_name(filename, &cur_dentry) == -1) { //check if successfullly open a file
          return -1;
      }
       //starting at two since in and out cannot be modified total size is 8
      for (index_for_fdt = 2; index_for_fdt < 8; index_for_fdt++) {
        if (pcb->fdt[index_for_fdt].flags == 0) {
          pcb->fdt[index_for_fdt].file_operation_table_ptr = find_table_point(cur_dentry.file_type);
          pcb->fdt[index_for_fdt].file_operation_table_ptr->open(filename);
          if (cur_dentry.file_type == 2) { //only has inode when data file
            pcb->fdt[index_for_fdt].inode = cur_dentry.inode_index;
          }
          else{
              pcb->fdt[index_for_fdt].inode = 0; //for rtc and dir
          }
          pcb->fdt[index_for_fdt].position = 0; // 0 for open update when read
          pcb->fdt[index_for_fdt].flags = 1; // 1 is in use , 0 is not use
          return index_for_fdt; //success fill in one file, return the fd number
        }
      }
      return -1; //fdt is full

}

/*
 *   close (int32_t fd)
 *   DESCRIPTION: close file and delete in fdt
 *   INPUTS: int32_t fd
 *   OUTPUTS: none
 *   RETURN VALUE: -1 fail 0 success
 */
int32_t close (int32_t fd){

  // get access to the current pcb
  pcb_t* pcb = get_cur_pcb();

  if (fd < 2 || fd >= 8 ) {
    return -1; // ?? invalid fd
  }
  if (pcb->fdt[fd].flags == 0) {
    return -1; //  resourse is not avaliable
  }
  if (pcb->fdt[fd].file_operation_table_ptr == NULL) {
    return -1; // function doesn't exist
  }

  pcb->fdt[fd].flags = 0; //delete the file in fdt
  return pcb->fdt[fd].file_operation_table_ptr->close(fd); //close the file
}


/*
 *   getargs
 *   DESCRIPTION: get the argument of the command
 *   INPUTS: buf, nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: -1 fail 0 success
 */
int32_t getargs(uint8_t* buf, int32_t nbytes){
  //sanity check
  if (buf == NULL || nbytes <= 0) {  
    return -1;
  }
  
  // get access to the current PCB
  pcb_t* pcb = get_cur_pcb();

  // check if there are agruments. If not, return -1.
  if (pcb->argument[0] == '\0') {
    return -1;
  }

  int i = 0;  // loop index
  while (pcb->argument[i] != '\0') {
    buf[i] = pcb->argument[i];
    i += 1;
  }
  buf[i] = '\0';

  return 0;
}

/*
 *   vidmap
 *   DESCRIPTION:
 *   INPUTS:
 *   OUTPUTS:
 *   RETURN VALUE:
 */
int32_t vidmap (uint8_t** screen_start){
      if (screen_start == NULL)
      {
        return -1;
      }

      // check if the argument is valid
      if ((int32_t)screen_start >= 0x8000000 && (int32_t)screen_start < 0x8400000) {
        pcb_t* pcb = get_cur_pcb();
        paging_set_up(pcb->pid);
        *screen_start = (uint8_t *)vidmap_paging_set_up();   // video memory starts at 132MB
        return 0;
      }
      else{
        return -1;
      }
}

/*
 *   set_up_io_file
 *   DESCRIPTION: set the fdt entries for stdin and stdout 
 *   INPUTS: pcb
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
void set_up_io_file(pcb_t* pcb){
  //stdin
  pcb->fdt[0].file_operation_table_ptr = find_table_point(3);//which is terminal_func
  pcb->fdt[0].inode = 0;
  pcb->fdt[0].position = 0; // 0 for open update when read
  pcb->fdt[0].flags = 1;
  //stdout
  pcb->fdt[1].file_operation_table_ptr = find_table_point(4);//which is terminal_func
  pcb->fdt[1].inode = 0;
  pcb->fdt[1].position = 0; // 0 for open update when read
  pcb->fdt[1].flags = 1;
}

/*
 *   get_cur_pcb 
 *   DESCRIPTION: get PCB through the value in ESP
 *   INPUTS: none
 *   OUTPUTS: current pcb 
 *   RETURN VALUE: pointer to the current pcb
 */
pcb_t* get_cur_pcb(void) {
  int tmp;  // temporary variable to hold the value in ESP
  asm volatile( "movl %%esp, %%eax;"   
                "addl $-4, %%eax;"   // avoid the case that esp points to the bottom of the 8 KB stack
                "movl %%eax, %0;"
                 :"=r" (tmp)
                 : /* no inputs */
                 : "%eax"
              );
  pcb_t* pcb = (pcb_t *)(tmp & PCB_MASK); // And with the bit mask to get addr of PCB
  return pcb;
}

//       // get the current id of the process
//       // 0x800000 is the addr of 8MB, 0x2000 is the addr of 8KB
//       cur_pid = ((int)pcb - 0x800000) / 0x2000 - 1;

