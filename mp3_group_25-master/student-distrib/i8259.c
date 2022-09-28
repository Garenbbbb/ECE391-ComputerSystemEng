/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"
#include "types.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7  */
uint8_t slave_mask = 0xFF;  /* IRQs 8-15 */

/*
 * i8259_init(void)
 *   DESCRIPTION: initialization of the pic
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: None
 *   SIDE EFFECTS: initialize the pic
 */
void i8259_init(void) {

cli();

  master_mask = 0xFF;
  slave_mask = 0xFF;
   outb(ICW1, MASTER_8259_PORT); //ICW1 select master to init
   outb( ICW2_MASTER  , 0x21);  //ICW2 IR0-7 mapped to x020-0x27
   outb(ICW3_MASTER, 0x21); //ICW3
   outb(0x01, 0x21); // normal EOI


   outb(ICW1, SLAVE_8259_PORT); //ICW1 slave initialization
   outb( ICW2_SLAVE, 0xA1); //ICW2 slave ir0-7 mapped to 0x28-0x2f
   outb(ICW3_SLAVE, 0xA1); // the ir on master is IR2
   outb(0x01, 0xA1); // slave's support for AEOI
   outb( 0xFF , MASTER_8259_PORT +1); //mask all of master
   outb( 0xFF ,SLAVE_8259_PORT+1); //mask all of slave
   //enable
   enable_irq(2);
sti();
   //udelay(100);

}
/*
 * enable_irq(uint32_t irq_num)
 *   DESCRIPTION:  Enable (unmask) the specified IRQ
 *   INPUTS: uint32_t irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: None
 *   SIDE EFFECTS: enable specific irq
     Reference: code in osdev
 */
void enable_irq(uint32_t irq_num) {
  uint16_t port;
  uint8_t value;

  if (irq_num < 0 || irq_num > 15) { //out of bound
    return;
  }

  if (irq_num < 8) {
    port = MASTER_8259_PORT + 1;
  }
  else {
    port = SLAVE_8259_PORT + 1;
    irq_num -= 8;
  }
  value = inb(port) & ~(1 << irq_num);
  outb(value, port);
}

/*
 * disable_irq(uint32_t irq_num)
 *   DESCRIPTION:  disable (unmask) the specified IRQ
 *   INPUTS: uint32_t irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: None
 *   SIDE EFFECTS: disable specific irq
     Reference: code in osdev
 */
void disable_irq(uint32_t irq_num) {
  uint16_t port;
  uint8_t value;

  if (irq_num < 0 || irq_num > 15) { //out of bound
    return;
  }
  
  if (irq_num < 8) { //master
    port = MASTER_8259_PORT + 1;
  }
  else {
    port = SLAVE_8259_PORT + 1;
    irq_num -= 8; //slave
  }
  value = inb(port) | (1 << irq_num);
  outb(value, port);

}


/*
 * send_eoi(uint32_t irq_num)
 *   DESCRIPTION:  Send end-of-interrupt signal for the specified IRQ
 *   INPUTS: uint32_t irq_num
 *   OUTPUTS: none
 *   RETURN VALUE: None
 *   SIDE EFFECTS: send eoi signal 
 */
void send_eoi(uint32_t irq_num) {
  if (irq_num < 0 || irq_num > 15) { //out of bound
    return;
  }
  if(irq_num >= 8){ //slave
	  	outb(EOI | (irq_num - 8), SLAVE_8259_PORT);
      outb(EOI + 2, MASTER_8259_PORT); // slave port connect master port is 2
  }
  else{
	   outb(EOI | irq_num, MASTER_8259_PORT); //master
  }

}
