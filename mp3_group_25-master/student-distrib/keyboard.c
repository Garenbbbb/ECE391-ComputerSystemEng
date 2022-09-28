/************************/

#include "keyboard.h"
#include "i8259.h"

#include "terminal.h"
#include "types.h"

#define KEYBOARD_IRQ 1

#define F1 0x3B
#define F2 0x3C
#define F3 0x3D
#define ALT 0x38

//set every flag to zero
int left_shift_flag = 0;
int right_shift_flag = 0;
int caps_flag = 0;
int alt_flag = 0;
int tab_flag = 0;
int ctrl_flag = 0;
uint8_t flag;

volatile int enter_flag = 0;
volatile uint8_t kb_buf_arr[BUF_SIZE];
volatile int buf_idx;


/*initialize the scan code array
    according to OSdev keyboard
    https://urldefense.com/v3/__https://wiki.osdev.org/PS/2_Keyboard__;!!DZ3fjg!uA0ceuBb8tQvig1JoeTuju5wDTVFBPk4oFUEFwd39Xjj31Q2xEY43177D3wp8G9f4bnA$ 
    code set 1. scan code is index and 
    the char are mapped into an array.
    things that don't need to print in mp3.1
    are mapped as 0
*/

unsigned char scan_code[MODE][SCAN_SIZE] = 
{{
    '\0','\0',//escape
    '1','2','3','4','5','6','7','8','9','0',
    '-','=',
    0,//backspace
    ' ',//tab
    'q','w','e','r','t','y','u','i','o','p','[',']',
    '\n',//enter
    0,//left_ctrl
    'a','s','d','f','g','h','j','k','l',';',
    '\'','`',
    0,//left shift
    '\\',
    'z','x','c','v',
    'b','n','m',',',
    '.','/',
    0,//right shift
    0,//kepad*
    0,//left alt
    ' ',//space
    0,//capslock
    0,//F1
    0,//F2
    0,//F3
    0,0,//F45
    0,0,0,0,//F6789
    0,//F10
    0,//numlock
    0,//scrolllock
    0,0,0,0,0,//F numbers
    0,0,0,0,
    0,0,0,0,
    0,0,0,
    0,//F11
    0,//F12
    0,0,0
},
{//shift
    0,0,//escape
    '!','@','#','$','%','^','&','*','(',')',
    '_','+',
    0,//backspace
    ' ',//tab
    'Q','W','E','R','T','Y','U','I','O','P','{','}',
    '\n',//enter
    0,//left_ctrl
    'A','S','D','F','G','H','J','K','L',':',
    '\"','~',
    0,//left shift
    '|',//'\'
    'Z','X','C','V','B','N','M',
    '<','>','?',
    0,//right shift
    0,//kepad*
    0,//left alt
    ' ',//space
    0,//capslock
    0,//F1
    0,0,0,0,//F2345
    0,0,0,0,//F6789
    0,//F10
    0,//numlock
    0,//scrolllock
    0,0,0,0,0,//F numbers
    0,0,0,0,
    0,0,0,0,
    0,0,0,
    0,//F11
    0,//F12
    0,0,0
},
{//capslock on
    0,0,//escape
    '1','2','3','4','5','6','7','8','9','0',
    '-','=',
    0,//backspace
    ' ',//tab
    'Q','W','E','R','T','Y','U','I','O','P','[',']',
    '\n',//enter
    0,//left_ctrl
    'A','S','D','F','G','H','J','K','L',';',
    '\'','`',
    0,//left shift
    '\\',//'\'
    'Z','X','C','V','B','N','M',
    ',','.','/',
    0,//right shift
    0,//kepad*
    0,//left alt
    ' ',//space
    0,//capslock
    0,//F1
    0,0,0,0,//F2345
    0,0,0,0,//F6789
    0,//F10
    0,//numlock
    0,//scrolllock
    0,0,0,0,0,//F numbers
    0,0,0,0,
    0,0,0,0,
    0,0,0,
    0,//F11
    0,//F12
    0,0,0
},
{//capslock and shift on
    0,0,//escape
    '!','@','#','$','%','^','&','*','(',')',
    '_','+',
    0,//backspace
    ' ',//tab
    'q','w','e','r','t','y','u','i','o','p','{','}',
    '\n',//enter
    0,//left_ctrl
    'a','s','d','f','g','h','j','k','l',':',
    '\"','~',
    0,//left shift
    '|',//'\'
    'z','x','c','v','b','n','m',
    '<','>','?',
    0,//right shift
    0,//kepad*
    0,//left alt
    ' ',//space
    0,//capslock
    0,//F1
    0,0,0,0,//F2345
    0,0,0,0,//F6789
    0,//F10
    0,//numlock
    0,//scrolllock
    0,0,0,0,0,//F numbers
    0,0,0,0,
    0,0,0,0,
    0,0,0,
    0,//F11
    0,//F12
    0,0,0
}};


//functions
/*  keyboard_init() :
    initialize the keyboard
    by calling enable irq.
    also initialize the flag
    input:none
    output:none
*/
void keyboard_init () {
    left_shift_flag = 0;
    right_shift_flag = 0;
    caps_flag = 0;
    buf_idx = 0;
    enter_flag = 0;
    enable_irq(KEYBOARD_IRQ);
}

/*  uint8_t check_mode(unsigned char code) :
    check the flags of shift, caps, enter, alt,ctrl
    if any is pressed or released, change flag
    input:code, the index in OSdev table
    output:0 or 1. 0 means flags has changed.
        1 means we have an output character later on
*/
uint8_t check_mode(unsigned char code) {
    if(code == 0x36) {//left shift pressed
        right_shift_flag = 1;
    } else if (code == 0x2A)  {//right shift pressed
        left_shift_flag = 1;
    } else if(code == 0xAA) {//left shift released
        left_shift_flag = 0;
    } else if(code == 0xB6) {//right shift released
        right_shift_flag = 0;
    } else if(code == 0x3A) {//capslock pressed
        //If already pressed, changed to 0
        //vice versa
        if (caps_flag == 0){
            caps_flag = 1;
        }else{
            caps_flag = 0;
        }
    } else if(code == 0x38) {//alt pressed
        alt_flag = 1;
    } else if(code == 0xB8) {//alt released
        alt_flag = 0;
    } else if(code == 0x1D) {//ctrl pressed
        ctrl_flag = 1;
    } else if(code == 0x9D) {//ctrl released
        ctrl_flag = 0;
    } else if(code == 0x1C) {//enter pressed
        if(code < BUF_SIZE) {
            //kb_buf_arr[buf_idx] = '\n';
            term[cur_term_idx].term_buf_arr[term[cur_term_idx].term_buf_idx] = '\n';  //store new line into array buffer
            //buf_idx++;
            enter();
            term[cur_term_idx].term_enter_flag = 1;// change the flag to opposite
            // putc('\n');
            return 1;
        }
    } else if(code == 0x0E) {//backspace pressed
        if(code > 0 && term[cur_term_idx].term_buf_idx > 0) {
            //kb_buf_arr[buf_idx] = ' ';
            term[cur_term_idx].term_buf_arr[term[cur_term_idx].term_buf_idx] = ' ';
            //buf_idx--;
            term[cur_term_idx].term_buf_idx -- ;
            backspace();
        }
    } else if(alt_flag == 1 && code == F1) {
        terminal_switch(cur_term_idx, 0);
        cur_term_idx = 0;
    } else if(alt_flag == 1 && code == F2) {
        terminal_switch(cur_term_idx, 1);
        cur_term_idx = 1;
    } else if(alt_flag == 1 && code == F3) {
        terminal_switch(cur_term_idx, 2);
        cur_term_idx = 2;
    } else {
        return 1;
    }
    return 0;
}


/*  show_pressed_key() :
    print the pressed key
    input:none
    output:none
    the function use inb to get the pressed
    key and use printf to print onto screen
*/
void show_pressed_key () {
    unsigned char input;
    unsigned char code;
    // uint8_t flag;
    //inb is a function in lib.h
    //that takes KB_DATA = 0x60
    //as a input and output the 
    //index of the pressed key in
    //scan_code table
    code = inb(KB_DATA);
    flag = check_mode(code);
    if (code >= SCAN_SIZE || code == NULL) {
        //tell system interrupt stop
        send_eoi(KEYBOARD_IRQ);
        return;
    }
    //if only ctrl and L is pressed, clear the screen
    if(ctrl_flag == 1 && left_shift_flag == 0 && right_shift_flag == 0 && caps_flag == 0 && code == 0x26) {
        //clear the input here
        clear();
        send_eoi(KEYBOARD_IRQ);
        return;
    }

    if (buf_idx < BUF_SIZE - 1) {
        
        if(flag == 1) {
            if(left_shift_flag == 0 && right_shift_flag == 0 && caps_flag == 0) {
                //no shift no caps
                input = scan_code[0][code];
            } else if((left_shift_flag == 1 || right_shift_flag == 1)  && caps_flag == 0) {
                //only shift
                input = scan_code[1][code];
            } else if(left_shift_flag == 0 && right_shift_flag == 0  && caps_flag == 1) {
                //only caps
                input = scan_code[2][code];
            } else if((left_shift_flag == 1 || right_shift_flag == 1)  && caps_flag == 1) {
                //shift and caps
                input = scan_code[3][code];
            }
            if(input != 0) {
                // kb_buf_arr[buf_idx] = input;
                // buf_idx++;
                term[cur_term_idx].term_buf_arr[term[cur_term_idx].term_buf_idx] = input;
                term[cur_term_idx].term_buf_idx ++;
                putc_switch(input);
            }
        }
        
    }
    
    

    //tell system interrupt stop
    send_eoi(KEYBOARD_IRQ);
 }


