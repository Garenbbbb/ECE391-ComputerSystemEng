/* header file for idt.c
*set up sturctures and define variables here
*also build up linkage with other files include linkage handler and descriptor file*/

#include "x86_desc.h"
#include "exception_handler.h"
#include "keyboard.h"
#include "rtc.h"
#include "scheduling.h"

// #include "keyboard.h"


extern void idt_init(); //initialize IDT
// extern void key_intr();
extern void do_irq(int irq);
void key_intr_asm();
void rtc_intr_asm();
void system_call_asm();
void pit_intr_asm();
