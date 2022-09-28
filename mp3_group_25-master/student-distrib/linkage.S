.text
/*This function is the linkage file for pit interrupt
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.globl pit_intr_asm         
pit_intr_asm:               
    pushal          # callee save registers    
    pushfl          # callee save flags
    pushl $32        # argument for rtc irq (40 is the idt number)         
    call do_irq
    addl $4, %esp   # pop argument
    popfl                   
    popal                   
    iret        # return 

/*This function is the linkage file for keyboard interrupt
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call keyboard intr key pressed function in do_irq */
.globl key_intr_asm         
key_intr_asm:               
    pushal          # callee save registers    
    pushfl          # callee save flags
    pushl $33        # argument for keyboard irq (33 is the idt number)         
    call do_irq
    addl $4, %esp   # pop argument
    popfl                   
    popal                   
    iret        # return 

/*This function is the linkage file for rtc interrupt
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.globl rtc_intr_asm         
rtc_intr_asm:               
    pushal          # callee save registers    
    pushfl          # callee save flags
    pushl $40        # argument for rtc irq (40 is the idt number)         
    call do_irq
    addl $4, %esp   # pop argument
    popfl                   
    popal                   
    iret        # return 


/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_de_asm 
    exception_de_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $0        # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_db_asm 
    exception_db_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $1        # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_nmi_asm
    exception_nmi_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $2        # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_bp_asm 
    exception_bp_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $3        # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_of_asm 
    exception_of_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $4       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_br_asm 
    exception_br_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $5       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_ud_asm 
    exception_ud_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $6       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_nm_asm 
    exception_nm_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $7       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_df_asm 
    exception_df_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $8       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_mf_asm 
    exception_mf_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $9       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_ts_asm 
    exception_ts_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $10       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_np_asm 
    exception_np_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $11       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_ss_asm 
    exception_ss_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $12       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_gp_asm 
    exception_gp_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $13       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_pf_asm 
    exception_pf_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $14       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_fp_asm 
    exception_fp_asm:    pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $16       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_ac_asm 
    exception_ac_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $17       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_mc_asm 
    exception_mc_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $18       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

/*This function is the linkage file for exception
*call do irq with argument when called
*Input:NONE
*Output: NONE
*Effect: Call rtc intr function in do_irq */
.global exception_xf_asm 
    exception_xf_asm:
        pushal          # callee save registers    
        pushfl          # callee save flags
        pushl $19       # argument for keyboard irq (33 is the idt number)         
        call do_irq
        addl $4, %esp   # pop argument
        popfl                   
        popal                   
        iret        # return 

