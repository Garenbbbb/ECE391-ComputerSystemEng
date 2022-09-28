.text

/*This function is the linkage for system call
*Input:NONE
*Output: NONE
*Effect: system call to each function*/
.globl system_call_asm
system_call_asm:

pushl %edi # callee save
pushl %esi
pushl %ebx


pushl %edx  # args
pushl %ecx
pushl %ebx

cmpl $1, %eax
jl invald_num # invalid sys call num < 1
cmpl $10, %eax
jg invald_num # invalid sys call num > 10

sti
call *jump_table( , %eax,4)  # jump_table
jmp finish

invald_num:
    movl $-1, %eax  # invalid -> ret -1


finish:
    addl $12, %esp # del 3 arg from stack

    popl %ebx #callee restore
    popl %esi
    popl %edi

    iret        # return to user space



jump_table:
.long  0, halt, execute, read, write, open, close, getargs, vidmap  # order!
