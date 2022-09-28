#include "types.h"
#include "lib.h"

//the size of scan code array
#define SCAN_SIZE 92
//the input of function inb in lib.h
#define KB_DATA 0x60
//the max size of buffer
#define BUF_SIZE 128
//four different conditions no shift and caps,
//only shift, only caps, shift and caps
#define MODE 4

//global variables
extern volatile uint8_t kb_buf_arr[BUF_SIZE];

extern volatile int buf_idx;

extern volatile int enter_flag;

//initialize the keyboard
//    by calling enable irq.
extern void keyboard_init();

//print the pressed key
extern void show_pressed_key();

extern uint8_t check_mode(unsigned char code);

