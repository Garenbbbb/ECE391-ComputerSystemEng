#include "rtc.h"
#include "lib.h"
#include "i8259.h"

#define RTC_IRQ  8
#define MAX_HZ   1024

/* counter used to record the number of interrupts received in RTC handler */
volatile int freq_ctr = 0;

/* freq_val number of interrupts at the highest frequency is equivalent to 1
   interrupt at the frequency the process wants  */
volatile int freq_val = 1;

/*
 * rtc_init
 *   DESCRIPTION: Initialize RTC to send periodic interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: 
 *   Reference: code in osdev              
 */   
void rtc_init() {
    char prev;
    
    enable_irq(RTC_IRQ);

    outb(0x8B, 0x70); //port number
    prev = inb(0x71); //port number 
    outb(0x8B, 0x70); //port number
    outb(prev | 0x40, 0x71); //port number

}

/*
 * rtc_open
 *   DESCRIPTION: Open a new instance of virtualized RTC
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:              
 */   
int32_t rtc_open(const uint8_t* filename) {
    /* initialize counter to 0 and frequency to 2Hz */
    freq_ctr = 0;
    freq_val = MAX_HZ / 2;  /* freq is initialized to 2Hz */

    return 0;
}

/*
 * rtc_close
 *   DESCRIPTION: Close the specified virtualized RTC
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:               
 */   
int32_t rtc_close(int32_t fd) {
    /* just return 0 for checkpoint 2 */
    return 0;
}

/*
 * rtc_read
 *   DESCRIPTION: Return 0 after an interrupt has occurred
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:               
 */   
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytesd) {
    while(1) {
        if (freq_ctr == freq_val) {
            break;
        }
    }
    
    /* reset counter to 0  */
    freq_ctr = 0;

    return 0;
}

/*
 * rtc_write
 *   DESCRIPTION: accept a 4-byte integer specifying the interrupt rate in Hz, 
 *                and set the rate of periodic interrupts accordingly.
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS:               
 */   
int32_t rtc_write(int32_t fd,  const void* buf, int32_t nbytes) {
    int32_t rate = ((int32_t* )buf)[0];

    /* check whether the rate is out of the bound 2 to 1024  */
    if (rate > 1024 || rate < 2) return -1;

    /* check whether the rate is a power of 2  */
    if (rate & (rate - 1))  return -1;

    /* calculate the corresponding freq_val */
    freq_val = MAX_HZ / rate;

    /* reset counter to 0 for the new frequency  */
    freq_ctr = 0;

    return 0;
}

/*
 * rtc_handler
 *   DESCRIPTION: Handle the rtc functionality 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: 
 *   Reference: code in osdev         
 */   
void rtc_handler() {
    /* increment the counter  */
    freq_ctr += 1;

    outb(0x0C, 0x70);//port number
    inb(0x71);//port number
    send_eoi(RTC_IRQ);
}

