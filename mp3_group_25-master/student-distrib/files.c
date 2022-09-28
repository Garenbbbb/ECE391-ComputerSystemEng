#include "files.h"

dentry_t cur_dentry; //record the dentry of file
dentry_t cur_dir_dentry; //record the dentry of dir

// function jump tables for each file type
function_jump_table_t file_func = {file_read, file_write, file_open, file_close};

function_jump_table_t rtc_func = {rtc_read, rtc_write, rtc_open, rtc_close};

function_jump_table_t dir_func = {dir_read, dir_write, dir_open, dir_close};

function_jump_table_t terminal_func_in  = {terminal_read, write_for_in, terminal_open, terminal_close};

function_jump_table_t terminal_func_out = {read_for_out, terminal_write, terminal_open, terminal_close};

/*
 * file_init
 *   DESCRIPTION: initiate the boot block, inode and data block
 *   INPUTS:none
 *   OUTPUTS: boot_block, inode and data block address
 *   RETURN VALUE: none
 *   SIDE EFFECTS: initializes the file info
 */
void file_init(uint32_t * starting_addr) {
    //init the boot_block
    boot_block = (boot_block_t*) starting_addr;
}

/*
 * file_open
 *   DESCRIPTION: open the file
 *   INPUTS: fname
 *   OUTPUTS:none
 *   RETURN VALUE: 0 if succeed, -1 if the named file doesn't exist
 *   SIDE EFFECTS: none
 */
int32_t file_open(const uint8_t* fname){
  if (boot_block == NULL) {
    return -1; //fs is not initialized
  }
  // check if read the directory entry of the file succeed
  if (0 == read_dentry_by_name(fname, &cur_dentry)) {
    if (cur_dentry.file_type == 2) {// regular file type is 2
       return 0;
    }
  }
  return -1;
}


/*
 * read_dentry_by_name
 *   DESCRIPTION: fill the entry if has this file name
 *   INPUTS: const uint8_t* fname, dentry_t* dentry
 *   OUTPUTS:none
 *   RETURN VALUE: 0 if has this file, -1 if does not have the file
 *   SIDE EFFECTS: none
 */
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry){
   uint32_t index_dir_entry;
   int name_count1;
   int name_count2;

   /* get the length of the file name, which is up to 32 characters */
   if(strlen((const int8_t*)fname) > 32){return -1;}
   else{name_count1 = strlen((const int8_t*)fname);}

   /* loop through the boot_block's directory entries to find the file */
   for (index_dir_entry = 0; index_dir_entry < boot_block->num_dir_entry; index_dir_entry++) {
        name_count2 = strlen((const int8_t*) boot_block->dir_entries[index_dir_entry].fname);
        if (name_count2 > 32) {
          name_count2 = 32;
        }
        if (name_count1 < name_count2) {
          name_count1 = name_count2;
        }
        if (!strncmp((const int8_t*)fname, (const int8_t*) boot_block->dir_entries[index_dir_entry].fname, name_count1)) {
          *dentry = (boot_block->dir_entries[index_dir_entry]);
          return 0;
        }
  }
  return -1;   // return -1 if the named file doesn't exist
}


/*
 * file_close
 *   DESCRIPTION: close the file
 *   INPUTS: fd
 *   OUTPUTS:none
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
int32_t file_close(int32_t fd){
  if (boot_block == NULL) {
    return -1; //fs is not initialized
  }
  return 0;
}


/*
 * file_wirte
 *   DESCRIPTION: try to write to a file
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS:none
 *   RETURN VALUE: -1 because file system is read only
 *   SIDE EFFECTS: none
 */
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes){
  return -1;
}


/*
 * read_data
 *   DESCRIPTION: Reads length bytes of data starting from offset
 *                from file specified by inode into buf.
 *   INPUTS: inode, offset, buf, length
 *   OUTPUTS:none
 *   RETURN VALUE: -- number of bytes read
                   -- 0 if end of the file was already reached
                   -- -1 if inode is not within the valid range
 *   SIDE EFFECTS: none
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    if (inode > boot_block->num_inodes) {
      return -1;
    }

    uint32_t num_bytes = length; //how many we need to read
    inode_block_t * cur_inode =  (inode_block_t*) boot_block +(1+inode); //each inode is 4096
    uint32_t length_file_remain = cur_inode->length - offset; //how many I have left
    uint32_t block_index = (uint32_t)(offset / 4096); //which block in inode
    uint32_t data_block_index = cur_inode->data_block_list[block_index]; //which block in file system 0_index
    data_block_t *data_block_ptr = (data_block_t *) boot_block +(1+boot_block->num_inodes+data_block_index);  //starting addr of block
    uint32_t offset_in_one_block = offset % 4096;      // 4096 is the size of the data block

    uint32_t bytes_index = 0;
    uint32_t index_in_data;
    uint32_t loop_index = 0;

    while (num_bytes > 0 && length_file_remain > 0) {

      if (num_bytes < (4096 - offset_in_one_block)  && num_bytes <= length_file_remain) { // enough info in this block
        index_in_data = num_bytes;     // 4096 is the size of the data block

      }
      else if (num_bytes > length_file_remain && length_file_remain <(4096 - offset_in_one_block)) {
          index_in_data = length_file_remain;

      }
      else{
          index_in_data = (4096 - offset_in_one_block);   // 4096 is the size of the data block

      }


      for ( loop_index = 0; loop_index < index_in_data; loop_index++) { //map bytes in buf 
        buf[bytes_index] = (uint8_t)(data_block_ptr->data_block_starting_addr[offset_in_one_block]);
        offset_in_one_block++;
        bytes_index++;
      }

      num_bytes -= index_in_data;
      length_file_remain -= index_in_data;
      if (num_bytes > 0 && length_file_remain > 0) { //next block
        block_index += 1;
        data_block_index = cur_inode->data_block_list[block_index];
        data_block_ptr = (data_block_t *) boot_block +(1+boot_block->num_inodes+data_block_index);
        offset_in_one_block = 0;
      }
    }
    return bytes_index;
}


/*
 * file_read
 *   DESCRIPTION: reads n bytes of data from file into buf
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS:none
 *   RETURN VALUE: -- number of bytes read
                   -- -1 if read failed
 *   SIDE EFFECTS: none
 */
int32_t file_read(int32_t fd,  void* buf, int32_t nbytes){
  
  // check the arguments are valid
  if (buf == NULL || nbytes <= 0) {  
    return -1;
  }
  if (boot_block == NULL) {
    return -1; //fs is not initialized
  }
   
  // get PCB through the value in ESP
  pcb_t* pcb = get_cur_pcb();

  uint32_t inode_num = pcb->fdt[fd].inode;  // inode number
  uint32_t offset = pcb->fdt[fd].position;
  uint32_t bytes_ret_index;    // number of bytes read

  /* file found, read data from data block */
  bytes_ret_index = read_data(inode_num,  offset,  (uint8_t*)buf,  nbytes);

  pcb->fdt[fd].position += bytes_ret_index; //update file position in pcb in fdt

  return bytes_ret_index;
}


/*
 * dir_open
 *   DESCRIPTION: open directory
 *   INPUTS: fname
 *   OUTPUTS:none
 *   RETURN VALUE: 0 if succeed, -1 if no named directory
 *   SIDE EFFECTS: none
 */
int32_t dir_open(const uint8_t* fname){
  // check if read the directory entry of the directory succeed
  if (0 == read_dentry_by_name(fname, &cur_dir_dentry)) {
     if (cur_dir_dentry.file_type == 1) { //dir file type is 1
       return 0;
     }
  }
  return -1;
}


/*
 * dir_close
 *   DESCRIPTION: close directory
 *   INPUTS: fd
 *   OUTPUTS:none
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
int32_t dir_close(int32_t fd){
  if (boot_block == NULL) {
    return -1; //fs is not initialized
  }
  return 0;
}


/*
 * dir_write
 *   DESCRIPTION: write to a directory
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS:none
 *   RETURN VALUE: -1 because file system is read only
 *   SIDE EFFECTS: none
 */
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes){
  return -1;
}


/*
 * dir_read
 *   DESCRIPTION: read the name of a file in directory
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS:none
 *   RETURN VALUE: -- 1 if the last file was not reached
 *                 -- 0 if the last file was already reached
                   -- -1 if read failed
 *   SIDE EFFECTS: none
 */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes){  

  // check the argument is valid
  if (buf == NULL) {  
    return -1;
  }
  if(fd < 2 || fd > 7 ){return -1;}  // check the fd number is valid

  int i;  // loop index
  int length = 0;   // length of the filename read

  // get access to the current PCB
  pcb_t* pcb = get_cur_pcb();

  // get the file position in fdt in pcb
  uint32_t offset = pcb->fdt[fd].position;

  // If last file was already reached, return 0 
  if (offset >= boot_block->num_dir_entry) {
    return 0;
  }
 
  // read the directory entry of the file in the buffer
  read_dentry_by_index(offset, &cur_dir_dentry);
  
  // read the filename into the input buffer
  for(i = 0; i < 32; i++){
  ((char*) buf)[i] = cur_dir_dentry.fname[i];
  }

  pcb->fdt[fd].position += 1; //update file position in fdt in pcb

  // find the length of the filename
  for(i = 0; i < 32; i ++){    // 32 is the maximal length of the file names
    if(cur_dir_dentry.fname[i] != NULL){length ++;}
  }

  return length;  // return the length of the filename read
}


/*
 * read_dentry_by_index
 *   DESCRIPTION: read a directory entry with the index given
 *   INPUTS: index, dentry pointer
 *   OUTPUTS:none
 *   RETURN VALUE: -- 0 if succeed
                   -- -1 if failed
 *   SIDE EFFECTS: none
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    /* check if index out of bound  */
    if (boot_block->num_dir_entry < index) {
      return -1;
    }

    // set the current directory entry according to index
    cur_dir_dentry = (boot_block->dir_entries[index]);

    return 0;
}

/*
 *    find_table_point(uint32_t file_type)
 *   DESCRIPTION: assign right address of function table
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: address of function table
 */
function_jump_table_t* find_table_point(uint32_t file_type){
  if (file_type == 0) { //rtc file is 0
    return &rtc_func;
  }
  else if(file_type == 1){ // dir file is 1
    return &dir_func;
  }
  else if(file_type == 2){ // regular file is 2
    return &file_func;
  }
  else if(file_type == 3){
    return &terminal_func_in; //o.w. terminal
  }
  else if(file_type == 4) {
    return &terminal_func_out; //o.w. terminal
  }
  else return NULL;
}

/*
 *    write_for_in
 *   DESCRIPTION: special function made to return -1 for write function in stdin
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: -1
 */
int32_t write_for_in(int32_t fd, const void* buf, int32_t nbytes) {
  return -1;
}

/*
 *    read_for_out
 *   DESCRIPTION: special function made to return -1 for read function in stdout
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: -1
 */
int32_t read_for_out(int32_t fd, void* buf, int32_t nbytes) {
  return -1;
}

