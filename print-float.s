# Simple printf of float using bit pattern
# To compile, enter: gcc print-float.s -o print-float.exe -lmsvcrt
# To run, enter: ./print-float.exe


.section .data
msg:
    .string "The number is %f!\n"

.section .text
.extern printf
.globl main

main:
	subq	$40, %rsp  	# save 32 shadow space + 8 for 64bit alignment
                       	# 40 bytes + 8 bytes from printf = 48
                        # which is divisible by 16, and complies with 16 byte boundary  

	movabsq	$4614253070214989087, %rdx 	# bit pattern of float 3.14
	leaq	msg(%rip), %rcx 			# first arg for printf
	call	printf

	movl	$0, %eax	# exit code
	addq	$40, %rsp	# restore stack	
	ret
	
