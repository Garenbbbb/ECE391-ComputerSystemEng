/*This file contains the IDT information set up and initialization
*Linked to x86_desc file for IDT declaration
*linked to file for interrupt and exception handler*/

#include "idt.h"


/*IDT_INIT initializes the IDT vector table
* EFFECT: fill in the vector table with function pointer and 64 bits struct
* INPUT: NONE
* OUTPUT: NONE
*/
void idt_init(){
    int i;
    /*loop the idt array and fil in the struct*/
    for(i = 0;i < NUM_VEC;i ++){
        idt[i].seg_selector = KERNEL_CS; //set seg selector to kernel cs
        if(i < 15){ //first 15 entries are exceptions and set p to 1
            idt[i].present = 1; 
        }
        else if((i > 15)&(i < 20)){ //15 is reserved, other in 20 is exception, set p to 1
            idt[i].present = 1;
        }
        else if(i == 0x20){
            idt[i].present = 1;
        }
        else if(i == 33){ //33 is the keyboard interrupt
            idt[i].present = 1;
        }
        else if(i == 40){
            idt[i].present = 1;
        }
        else if(i == 0x80){
            idt[i].present = 1;
        }
        else{idt[i].present = 0;}
        idt[i].size = 1;//fill in the struct
        idt[i].dpl = 0;//fill in the struct
        idt[i].reserved0 = 0;//fill in the struct
        idt[i].reserved1 = 1;//fill in the struct
        idt[i].reserved2 = 1;//fill in the struct
        idt[i].reserved4 = 0;//fill in the struct
        idt[0x80].dpl = 3;
        if(i < 32){idt[i].reserved3 = 1;}//for the eception, reserved3 is 1
        else{idt[i].reserved3 = 0;}//for interrupt, trserved 3 is 0
    }

    //set the pointer to the exception in the offset
    SET_IDT_ENTRY(idt[0],exception_de);
    SET_IDT_ENTRY(idt[1],exception_db);
    SET_IDT_ENTRY(idt[2],exception_nmi);
    SET_IDT_ENTRY(idt[3],exception_bp);
    SET_IDT_ENTRY(idt[4],exception_of);
    SET_IDT_ENTRY(idt[5],exception_br);
    SET_IDT_ENTRY(idt[6],exception_ud);
    SET_IDT_ENTRY(idt[7],exception_nm);
    SET_IDT_ENTRY(idt[8],exception_df);
    SET_IDT_ENTRY(idt[9],exception_mf);
    SET_IDT_ENTRY(idt[10],exception_ts);
    SET_IDT_ENTRY(idt[11],exception_np);
    SET_IDT_ENTRY(idt[12],exception_ss);
    SET_IDT_ENTRY(idt[13],exception_gp);
    SET_IDT_ENTRY(idt[14],exception_pf);
    //SET_IDT_ENTRY(idt[15],eception_de); eception 15 is reserved
    SET_IDT_ENTRY(idt[16],exception_fp);
    SET_IDT_ENTRY(idt[17],exception_ac);
    SET_IDT_ENTRY(idt[18],exception_mc);
    SET_IDT_ENTRY(idt[19],exception_xf);

    SET_IDT_ENTRY(idt[0x20],pit_intr_asm);    

    SET_IDT_ENTRY(idt[33],key_intr_asm);//set the keyboard interrupt to linkage pointer
    SET_IDT_ENTRY(idt[40],rtc_intr_asm);

    SET_IDT_ENTRY(idt[0x80],system_call_asm);

}

/*do irq function is called when linkage assembly recieve interrupt from keyboard and rtc
* based on the argument, select keyboard or rtc intr and call 
* INPUT: irq number 
* OUTPUT: NONE
*  EFFECT: interrupt call
*/
void do_irq(int irq){
    switch(irq){
        case 0x20:
            pit_handler();
            break;
        case 33: //when irq = 33, keyboard interrupt raised
            show_pressed_key();
            break;
        case 40: //when irq = 40, rtc interrupt raised
            rtc_handler();
            break;
    }

    
}
