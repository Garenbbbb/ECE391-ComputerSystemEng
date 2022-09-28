#include "terminal.h"

term_struct_t term[TERM_NUM];
uint8_t cur_term_idx;
/* int32_t terminal_open (const uint8_t* filename)
 * Inputs:    file name
 * Return Value: 0
 * Function: open the terminal */
int32_t terminal_open (const uint8_t* filename) {
    return 0;
}


/* int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes)
 * Inputs:    file name which is  fd, 
            buf:array to be printed
                nbytes: numbe of bytes read
 * Return Value: number of bytes read
 * Function: map elements from keyboard buffer array to the
 terminal buffer array */
int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes) {
    // uint32_t index;
    int i;
    int count = 0;
    if(buf == NULL) {
        return -1;
    }
    if(term[cur_term_idx].term_buf_arr  == NULL) {
        return -1;
    }
    //when not enter, spin around
    while(term[cur_term_idx].term_enter_flag == 0)
    {
        
    }
    //reset the enter flag
    term[cur_term_idx].term_enter_flag = 0;
    //map the elementys
    for(i = 0; i < term[cur_term_idx].term_buf_idx; i++) {
        ((char*)buf)[i] = term[cur_term_idx].term_buf_arr[i] ;
        count++;
    }
    //clear the keyboard buffer array
    for (i = 0; i < term[cur_term_idx].term_buf_idx; i++) {
        term[cur_term_idx].term_buf_arr[i]  = ' ';
    }
    //reset the index
    term[cur_term_idx].term_buf_idx = 0;
    return count;
}

/* int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes)
 * Inputs:    file name which is  fd, 
            buf:array to be printed
                nbytes: numbe of bytes read
 * Return Value: nbytes
 * Function: print every element in the buf array */

int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes) {
    // printf("391OS>");
    int32_t i;
    for(i = 0; i < nbytes; i++) {
        putc(((char*)buf)[i]);
    }
    
    return nbytes;
}

/* int32_t terminal_close (int32_t fd)
 * Inputs:    file name which is  fd
 * Return Value: 0
 * Function: close the terminal */
int32_t terminal_close (int32_t fd) {
    return 0;
}

/* terminal_start 
 * Inputs:    
 * Return Value: 0
 * Function: initialize the terminal structures */
void terminal_start (){
    // initialize the terminal structures
    int i = 0;    // loop index
    int j = 0;
    cur_term_idx = 0;

    // loop to initialize each member in the struct
    for (i = 0; i < TERM_NUM; i++) {
        term[i].term_screen_x = 0;
        term[i].term_screen_y = 0;
        for(j = 0; j < BUF_SIZE; j++) {
            term[i].term_buf_arr[j] = 0;
        }
        term[i].term_buf_idx = 0;
        term[i].term_enter_flag = 0;
        term[i].pcb_idx = 0;
    }
    //assign memory address
    term[0].video_mem = (char *)0xB9000;  // this is the addr of video mem of the first terminal
    term[1].video_mem = (char *)0xBA000;  // this is the addr of video mem of the first terminal
    term[2].video_mem = (char *)0xBB000;  // this is the addr of video mem of the first terminal
    
}

/* terminal_save
 * Inputs:    
 * Return Value: 0
 * Function: save b8 video to terminal struct */
int32_t terminal_save (int32_t term_idx){
    cur_term_idx = term_idx;
    term[cur_term_idx].term_screen_x = get_x();
    term[cur_term_idx].term_screen_y = get_y();
    memcpy(term[cur_term_idx].video_mem, (void*)VIDEO, 4096); //4kb
    return 0;
}

/* terminal_load
 * Inputs:    
 * Return Value: 0
 * Function: load terminal struct video mem to b8 */
int32_t terminal_load (int32_t term_idx) {
    cur_term_idx = term_idx;
    update_cursor(term[cur_term_idx].term_screen_x, term[cur_term_idx].term_screen_y);
    memcpy((void*)VIDEO, term[cur_term_idx].video_mem, 4096); //4kb
    return 0;

}

/* terminal_switch
 * Inputs:    
 * Return Value: 0
 * Function: switch terminal */
void terminal_switch (int32_t term_prev, int32_t term_cur) {
    terminal_save(term_prev);
    
// get current pcb (old)
    // pcb_t * cur_pcb = (pcb_t *)(PIT_EIGHT_MB - (term[term_prev].pcb_idx+1) * PIT_EIGHT_KB);

    // // //save this esp ebp to old pcb
    // asm volatile(
    //                 "movl %%esp, %0;"
    //                 "movl %%ebp, %1;"
    //                 :"=a" (cur_pcb->esp), "=b"(cur_pcb->ebp)
    //                 :  // no inputs
    //                 );

    terminal_load(term_cur);


    // pcb_t * next_pcb = (pcb_t *)(PIT_EIGHT_MB - (term[term_cur].pcb_idx+1) * PIT_EIGHT_KB);
    // // // flush tlb
    // paging_set_up(term[term_cur].pcb_idx);

    // // //load next pcb esp ebp to register
    // asm volatile(
    //                "movl %0, %%esp;"
    //                "movl %1, %%ebp;"
    //                :  // no outputs
    //                :"a"(next_pcb->esp), "b"(next_pcb->ebp)
    //                );

}
