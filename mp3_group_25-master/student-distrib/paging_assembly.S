
.text

.globl loadPageDirectory
.globl enablePaging


loadPageDirectory:
   # Setup stackframe
   pushl %ebp
   movl %esp, %ebp

   # Load address of page directory into the CR3 register
   movl 8(%esp), %eax
   movl %eax, %cr3

   # Enable PSE (4 MiB pages)
   movl %cr4, %eax
   orl $0x00000010, %eax   #set the fourth bit to 1 to enable the 4MB page
   movl %eax, %cr4

   leave
   ret

enablePaging:
   # Setup stackframe
   pushl %ebp
   movl %esp, %ebp

   # Set the 32th bit in the CR0 register, the paging bit.
   movl %cr0, %eax
   orl $0x80000000, %eax  #set the 31th bit to 1 to enable the paging
   movl %eax, %cr0

   leave
   ret
 
