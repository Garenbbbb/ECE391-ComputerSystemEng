#ifndef SCHEDULING_H
#define SCHEDULING_H


#define PIT_IRQ  0
#define MODE_REG  0x43
#define PIT_MODE_3 0x36
#define PIT_CHANNEL_0  0x40
#define PIT_MASK  0xFF
#define PIT_FREQUENCY 1193182
#define PIT_EIGHT 8
#define HZ_VALUE 20
#define PIT_EIGHT_MB 0x800000
#define PIT_EIGHT_KB 0x2000

extern int schedule_arr[3];
extern int background_terminal;


/* initialize the pit*/
void init_pit();


/* pit pit_handler*/
void pit_handler();


/* do the context switch*/
void contextSwitch();


#endif

