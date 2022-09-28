#include "scheduling.h"
#include "paging.h"
#include "system_call.h"
#include "files.h"
#include "lib.h"
#include "types.h"
#include "i8259.h"


//schedule array
int schedule_arr[3] = {-1, -1, -1};
int background_terminal = -1;

// int base_shell_flag = 0;

/*
 *  init_pit()
 *   DESCRIPTION: initialize the pit for scheduling
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: None
 */
void init_pit(){
  //init the pit
  outb(PIT_MODE_3, MODE_REG);      // ?
  outb((PIT_FREQUENCY/HZ_VALUE) & PIT_MASK, PIT_CHANNEL_0);   //  low byte
  outb((PIT_FREQUENCY/HZ_VALUE) >> PIT_EIGHT, PIT_CHANNEL_0);     //  high byte
  
  enable_irq(PIT_IRQ); //enable interupt
  return;
}

/*
 *  pit_handler
 *   DESCRIPTION: handle pit interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: None
 */
void pit_handler(){
    send_eoi(PIT_IRQ); //send the eoi

    cli();

    contextSwitch(); //do the context switch

    sti();

    return;
}


/*
 *  contextSwitch()
 *   DESCRIPTION: do context switch when schduling
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: None
 */
void contextSwitch(){
// run shell program respectively for three tereminals
//map program image
// int cur_pid = schedule_arr[background_terminal];
// background_terminal = (background_terminal+1) % 3; // update to next background_terminal, 3 means there are 3 terminals in total
// int next_pid = schedule_arr[background_terminal]; // get next process

int i; 
for(i = 0;i < 3;i ++){
  if(schedule_arr[i] == -1){
    schedule_arr[i] = i;
    background_terminal = i;
    term[i].pcb_idx = i;
    execute((uint8_t*)"shell");
  }
}

// get current pcb (old)
// pcb_t * cur_pcb = (pcb_t *)(PIT_EIGHT_MB - (cur_pid+1) * PIT_EIGHT_KB);

// //save this esp ebp to old pcb
// asm volatile(
//                 "movl %%esp, %0;"
//                 "movl %%ebp, %1;"
//                 :"=a" (cur_pcb->esp), "=b"(cur_pcb->ebp)
//                 :  // no inputs
//                 );

// // get next pcb
// pcb_t * next_pcb = (pcb_t *)(PIT_EIGHT_MB - (next_pid+1) * PIT_EIGHT_KB);

// // tss  ????   next? old?
// tss.ss0 = KERNEL_DS;
// tss.esp0 = PIT_EIGHT_MB - (next_pid * PIT_EIGHT_KB) - 4;  // -4 to save space for esp

// // flush tlb
// paging_set_up(next_pid);

// //load next pcb esp ebp to register
// asm volatile(
//                "movl %0, %%esp;"
//                "movl %1, %%ebp;"
//                :  // no outputs
//                :"a"(next_pcb->esp), "b"(next_pcb->ebp)
//                );


return;
}
