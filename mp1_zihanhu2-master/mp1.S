
# mp1.S - missile-command version
# Good luck, and have fun!

.data

    # Constants for accessing the fields of a struct missile,
    # struct missile is defined in rtc.h and mp1.h

	NEXT 	  = 0
	X	  = 4
	Y	  = 8
	VX	  = 12
	VY	  = 16
	DEST_X	  = 20
	DEST_Y	  = 24
	EXPLODED  = 28
	C	  = 32

    # Character to draw for an explosion - '@'

	EXPLOSION = 64


# Data shared between this file and rtc.c helper functions

# This '.globl' directive makes these symbols visible externally
.globl mp1_missile_list, base_alive, mp1_score

mp1_missile_list:	.long 0x0	# Head of list pointer
base_alive:		.long 0x0	# Array of 3 bytes, plus a padding byte
mp1_score:		.long 0x0	# Player's current score



# Data private to this  file

base_pic:     	    .string "/^^^\\"	# Picture of a live base
dead_base_pic: 	    .string "xxxxx"	# Picture of a dead base
crosshairs_x:		.long 0x0	# X-position of the crosshairs
crosshairs_y:		.long 0x0	# Y-position of the crosshairs
base_score:		.long 0x0	# Place to store rv for get_status

.text

# void mp1_poke(void);
# You can use this function to write to video memory.
#
# Interface: Register-based arguments (not C-style)
#    Inputs: %cl  - The byte you wish to write
#            %eax - Offset from the start of video memory that you wish
#                   to write to
#   Outputs: Text-mode video screen is written to at location %eax with
#            the byte in %cl
# Registers: Clobbers EDX

mp1_poke:
		pushl %edx
		movl    vmem_base_addr(,1),%edx
		movb    %cl,(%edx,%eax,1)
		popl %edx
		ret




#   ----------------- Exported functions ---------------------

# void mp1_rtc_tasklet(unsigned long garbage);
# Performs three tasks:
#	(1) updates the list of missiles (implement this in update_missiles,
#           below, and call it from here).
#	(2) Redraw the bases - they may have been overwritten with missiles
#	(3) Redraw the crosshairs - it may have been overwritten with missiles
# Inputs   : none
# Outputs  : none
# Registers: Standard C calling convention

.globl mp1_rtc_tasklet
mp1_rtc_tasklet:
pushl %ebp
movl %esp, %ebp
pushl %edi #callee save
pushl %esi
pushl %ebx


pushl %ecx #caller save
call update_missiles
popl  %ecx #caller save


	movl %eax, %ebx  #save notify user r.v.
draw_cities:
	
	#redraw cities POS = 160X+2Y
	movl $24, %eax  # y pos of last row is 24
	imull $160, %eax #NUM OF MEMORY PER ROW
	movl base_alive, %edx
	movl $18, %ecx # x pos of start of first base
	imull $2, %ecx # 2 memory pre bit
	addl %ecx, %eax #start position for draw
	movb $3, %ch #counter for 3 city
city_loop:
	cmpb $0, %ch #counter = 0 
	jle draw_cross
	movl $0, %esi #counter for 5char
	cmpb $0, %dl # edx
	je base_dead
	movl $base_pic, %edi
	jmp continue
base_dead:
	movl $dead_base_pic, %edi
continue:
	cmpl $5, %esi #counter = 5 
	jge next_city
	movb $32, %cl #empty old city
	pushl %edx
	call mp1_poke
	popl %edx
	#draw new char
	pushl %ebx
	movl %esi, %ebx
	addl %edi, %ebx
	movl (%ebx), %ebx
	movb %bl, %cl
	pushl %edx
	call mp1_poke
	popl %edx
	popl %ebx
	addl $2, %eax #next location
	addl $1, %esi #next char
	jmp continue

next_city:
	shr $8, %edx 
	addl $32, %eax #next city starting point
	subb $1, %ch # dcrease city counter
	jmp city_loop

 draw_cross:
	#redraw cross_hair
	movl crosshairs_x, %eax
	imull $2, %eax # 2 memory for one position
	movl crosshairs_y, %edx
	imull $160, %edx  #NUM OF MEMORY PER ROW
	addl %edx, %eax
	movb $43 , %cl  # crosshair +
	call mp1_poke

	cmpl $1, %ebx  #r.v. if notify user
	jne not_notify
	call mp1_notify_user  #notify_user
not_notify:
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret #


# int mp1_ioctl(unsigned long arg, unsigned int cmd)
# The dispatch function for the MP1 ioctls - should use the cmd argument
# and a jumptable to execute one of the specific ioctls implemented below.
# Inputs   : unsigned long arg - parameter to the mp1_ioctl_....
#          : unsigned int cmd  - specifies which mp1_ioctl_... function
#          :                     to execute
# Outputs  : Returns an integer - depends on which ioctl() function is called
# Registers: Standard C calling convention

.globl mp1_ioctl
mp1_ioctl:

movl 8(%esp), %ecx #load cmd var to ecx 
cmpl $0 , %ecx #if cmd < 0
jl error_cmd
cmpl $4 , %ecx
jg error_cmd # if cmd > 4

jmp *jump_table(,%ecx,4)  #

error_cmd:
	movl $-1 , %eax
	ret


jump_table:
.long  mp1_ioctl_startgame, mp1_ioctl_addmissile,mp1_ioctl_movexhairs,mp1_ioctl_getstatus,mp1_ioctl_endgame


# ----------------- Functions private to this file -------------------
# int update missiles
# update the missile in the linklist and draw on the screen
# Inputs   : none
# Outputs  : Returns an integer - tell if we need notify the user
# Registers: Standard C calling convention

update_missiles:
pushl %ebp
movl %esp, %ebp
pushl %edi #callee save
pushl %esi
pushl %ebx

#loop each missile
movl  mp1_missile_list, %ecx  # load pointer of missile to ecx
cmpl $0, %ecx
je finish_return

loop:

  	movl X(%ecx), %eax #store old x
	movl Y(%ecx), %ebx	# store old y

	cmpl $0, EXPLODED(%ecx) # compare if exploded = 0
	jne case_2 #jmp if not equal 0

	

	movl VX(%ecx) , %edx # move speed to edx
	addl  %edx, X(%ecx)  # new x
	cmpl $0x004fffff, X(%ecx) # check if x out of bound 79
	jg erased  # go erase missile erase old
	cmpl $0, X(%ecx)
	jl erased

	movl VY(%ecx) , %edx # move speed to edx
	addl %edx,  Y(%ecx)  # new y
	cmpl $0x0018ffff, Y(%ecx)  # check if x out of bound 24
	jg erased #go erase missile
	cmpl $0, Y(%ecx)
	jl erased
	

check_des:	
	#check if reach destination
	movl DEST_Y(%ecx),%edx
	movl Y(%ecx), %edi
	shrl $16, %edi
	cmp %edx, %edi #comp new y with des_y
	jne draw_missile
  	movl DEST_X(%ecx),%edx
	movl X(%ecx) , %edi
	shrl $16, %edi
	cmpl %edx, %edi  #comp new x with des_x
	jne draw_missile

  case_2:   #explode or reach des remove the old symbol on the screen
	shrl $16, %eax #get upper 16bit
	imull $2, %eax #2 memory for one scrren location

	shrl $16, %ebx #get upper 16bit
	imull $160, %ebx  #NUM OF MEMORY PER ROW
	addl %ebx, %eax
	pushl %ecx
	pushl %esi
	movb $32 , %cl  # missile char 
	call mp1_poke
	popl %esi
	popl %ecx

	



	pushl %esi
	pushl %ecx
	pushl %ecx # argument for missile_explode
	call missile_explode	 # call missil_explode
	addl $4, %esp #pop argument
	popl %ecx
	popl %esi
	#return value check(status of game)
	cmpl $0 , %eax # check the return val of missile_explode
	je not_notify_user
	movl $1, %esi

not_notify_user:
	movl EXPLODED(%ecx), %edx # move exploded to edx
	subl $1, %edx #subtract exploded by 1
 	cmpl $0 , %edx # compare if exploded = 0
	jle erased_2 # if=0 erased 
	movl %edx, EXPLODED(%ecx)
#draw with explosion character
	movl X(%ecx), %eax
	shrl $16, %eax #get upper 16bit
	imull $2, %eax #2 memory per one location
	movl Y(%ecx), %edx
	shrl $16, %edx
	imul $160, %edx  #NUM OF MEMORY PER ROW
	addl %edx, %eax
	pushl %esi
	pushl %ecx
	movb $64, %cl #explosion char
	call mp1_poke

	popl %ecx
	popl %esi



	jmp Next_missile

erased:  #free missile

#remove from screen

	shrl $16, %eax #high 16
	imull $2, %eax

	shrl $16, %ebx
	imull $160, %ebx  #NUM OF MEMORY PER ROW
	addl %ebx, %eax
	pushl %ecx
	pushl %esi
	movb $32 , %cl  # missile char 
	call mp1_poke
	popl %esi
	popl %ecx
erased_2:
	#update ecx to next missile first
	movl mp1_missile_list,%edx
	cmpl %edx, %ecx
	jne not_first
	movl NEXT(%ecx), %edx
	movl %edx, mp1_missile_list
	jmp free_old
not_first:
	movl (%edx),%eax
	cmpl %eax, %ecx
	je free_2
	
	movl (%edx), %edx
	jmp not_first

free_old:
	pushl %edx
	pushl %ecx
	call mp1_free #free missile memory
	add $4, %esp
	popl %edx
	movl %edx, %ecx
	jmp Next_missile_1
free_2:
	movl (%ecx), %eax
	movl %eax, (%edx)
	pushl %eax
	pushl %ecx
	call mp1_free
	popl %ecx
	popl %eax
	movl %eax, %ecx
	jmp Next_missile_1

draw_missile:

				
	#not delete old missile if not change position
	shrl $16, %ebx
	shrl $16, %eax
	movl X(%ecx), %edx
	shrl $16, %edx
	cmpl %eax, %edx
	jne erase_old_mi
	movl Y(%ecx), %edx
	shrl $16, %edx
	cmpl %ebx, %edx
	jne erase_old_mi
	jmp draw_again


erase_old_mi:
	imull $2, %eax
	imull $160, %ebx
	addl %ebx, %eax
	pushl %ecx
	pushl %esi
	movb $32 , %cl  # missile char 
	call mp1_poke
	popl %esi
	popl %ecx



draw_again:
	movl X(%ecx), %eax
	shrl $16, %eax
	imull $2, %eax
	movl Y(%ecx), %edi
	shrl $16, %edi
	imull $160, %edi  #NUM OF MEMORY PER ROW
	addl %edi, %eax
	pushl %ecx
	pushl %esi
	movb C(%ecx) , %cl  # missile char 
	call mp1_poke
	popl %esi
	popl %ecx

Next_missile:
	movl NEXT(%ecx), %ecx # set ecx to next missile pointer
Next_missile_1:
	cmpl $0, %ecx #check if next is null
	jne loop #


	movl %esi, %eax
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
	finish_return:
	movl $0, %eax
popl %ebx
	popl %esi
	popl %edi
	leave
	ret






# mp1_ioctl_startgame
# initialize the game and set the varible to
# Inputs   : none
# Outputs  : Returns an integer - tell if we need notify the user
# Registers: Standard C calling convention


mp1_ioctl_startgame:
pushl %ebp
movl %esp, %ebp
pushl %esi
pushl %ebx
pushl %edi


	movl $40 , crosshairs_x # update crosshairx = 40
	movl $12 , crosshairs_y # update crosshairy = 12

	#update crosshairs to (40, 12)
	movl $0, mp1_score
	movl $0x00010101, base_alive
	popl %edi
	popl %ebx
	popl %esi
	leave
	ret


# mp1_ioctl_addmissile
# allocate location for new missile, get data from user, and put it in the linklist
# Inputs   : address from the user space for missile
# Outputs  : Returns an integer to tell if add correctly -1 : fail 0: success
# Registers: Standard C calling convention
mp1_ioctl_addmissile:
	pushl %ebp
	movl %esp, %ebp
	pushl %edi
	pushl %ebx
	pushl %esi

	movl $33, %ecx # allocate 33memory
	movl 8(%ebp), %edx
	pushl %edx
	pushl %ecx
	call mp1_malloc
	popl %ecx
	popl %edx

	pushl %ecx
	pushl %edx
	pushl %eax
	call mp1_copy_from_user
	cmpl $0, %eax
	jne copy_fail
	popl %eax
	addl $8, %esp
	movl mp1_missile_list, %edx
	movl %edx, NEXT(%eax)
	movl %eax, mp1_missile_list
	jmp copy_finish


 

copy_fail:
	popl %eax
	addl $8, %esp
	pushl %eax
	call mp1_free
	addl $4, %esp
	movl $-1, %eax
	popl %edi
	popl %ebx
	popl %esi
	leave
	ret 

copy_finish:
	movl $0, %eax
	popl %edi
	popl %ebx
	popl %esi
	leave
	ret 

#mp1_ioctl_movexhairs
# update the missile in the linklist and draw on the screen
# Inputs   :  32 bit integer upper 16 is change of x lower 16 is change of y 
# Outputs  : Returns an integer - always 0
# Registers: Standard C calling convention

mp1_ioctl_movexhairs:
	pushl %ebp
	movl %esp, %ebp
	pushl %edx
	pushl %esi
	pushl %ebx

	movl 8(%ebp), %edx #load arg to edx
	movl crosshairs_x, %ebx # load current x to ebx
	addw  %dx, %bx  #add 
	cmpl $0, %ebx
	jl fail_move
	cmpl $79, %ebx #if out of screen
	jg fail_move
	movl crosshairs_y, %eax # load current y to eax
	shr $16, %edx  # shift 16 bits 
	addw %dx, %ax #
	cmpl $0, %eax # y less than 0
	jl fail_move
	cmpl $24, %eax # y great than 24
	jg fail_move
	#undraw
	movl %eax, %edx
	movl crosshairs_x, %eax
	imull $2, %eax
	movl crosshairs_y, %esi
	imull $160, %esi  #NUM OF MEMORY PER ROW
	addl %esi, %eax
	movb $32 , %cl  # crosshair space
	pushl %edx
	call mp1_poke
	popl %edx
	movl $crosshairs_x, %eax
	movl %ebx, (%eax)
	movl $crosshairs_y, %eax
	movl %edx, (%eax)
	#draw
	movl crosshairs_x, %eax
	imull $2, %eax
	movl crosshairs_y, %edx
	imull $160, %edx  #NUM OF MEMORY PER ROW
	addl %edx, %eax
	movb $43 , %cl  # crosshair +
	call mp1_poke

fail_move:
	movl $0, %eax
popl %ebx
popl %esi
popl %edx
	leave
	ret




#mp1_ioctl_getstatus
#allow user receive the current score and status of three cities
# Inputs   :  address in the user space to store the info
# Outputs  : Returns an integer - 0-success -1-fail
# Registers: Standard C calling convention

mp1_ioctl_getstatus:
	pushl %ebp
movl %esp, %ebp
	pushl %edx
	pushl %esi

	movl 8(%ebp), %edx #load arg to edx
	movl $0, %eax # clear eax
	movl base_alive, %ecx
	movb %cl, %al  #third city
	shl  $1, %eax #left shift
	shr  $8, %ecx # put high 16 to low 16
	addb  %cl, %al #second city
	shl  $1, %eax
	addb %ch, %al # first city
	shl $16, %eax
	movl mp1_score, %ecx
	addl %ecx, %eax #add score to eax
	movl $base_score, %ecx #load addr of base_score to ecx
	movl %eax, (%ecx) #save to base_score

	pushl $4
	pushl %ecx
	pushl %edx
	call mp1_copy_to_user
	addl $12, %esp
	cmpl $0, %eax
	jne fail_save

	movl $0, %eax
popl %esi
popl %edx
	leave
	ret
fail_save:
	movl $-1, %eax
popl %esi
popl %edx
	leave
	ret


#mp1_ioctl_endgame
#empty all the space in memory we used for the missile linklist
# Inputs   : ignore
# Outputs  : integer
# Registers: Standard C calling convention

mp1_ioctl_endgame:
pushl %ebp
movl %esp, %ebp


	movl mp1_missile_list, %ecx
	movl $0, mp1_missile_list
loop_end:
	cmpl $0, %ecx
	je end_game
	movl %ecx, %edx
	movl NEXT(%ecx) , %ecx
	pushl %ecx
	pushl %edx
	call mp1_free
	add $4, %esp
	popl %ecx
	jmp loop_end


end_game:
	movl $0, %eax
	leave
	ret

