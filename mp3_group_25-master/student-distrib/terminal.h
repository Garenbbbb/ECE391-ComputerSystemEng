#ifndef TERMINAL_H
#define TERMINAL_H

#include "keyboard.h"
#include "files.h"
#include "paging.h"
// #include "lib.h"

#define TERM_NUM 3


int32_t terminal_open (const uint8_t* filename);

int32_t terminal_read (int32_t fd, void* buf, int32_t nbytes);

int32_t terminal_write (int32_t fd, const void* buf, int32_t nbytes);

int32_t terminal_close (int32_t fd);

// structure for terminals
typedef struct term_struct{
    /* data */
    //separate buffer array
    uint8_t term_buf_arr[BUF_SIZE];
    int term_buf_idx;
    //position of cursor
    int term_screen_x;
    int term_screen_y;
    //enter state
    int term_enter_flag;
    char* video_mem;
    int pcb_idx; //current terminal process index
}term_struct_t;

extern term_struct_t term[TERM_NUM];
 //current terminal index
extern uint8_t cur_term_idx;


void terminal_start ();

int32_t terminal_save (int32_t term_idx);

int32_t terminal_load (int32_t term_idx);

void terminal_switch (int32_t term_prev, int32_t term_cur);

#endif
