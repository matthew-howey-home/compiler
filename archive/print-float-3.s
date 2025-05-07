# Simple printf of float using float literals (actual float value in data) added together
# To compile, enter: gcc print-float-3.s -o print-float-3.exe -lmsvcrt
# To run, enter: ./print-float-3.exe


.section .data

msg:
	.string "The number is %f!"
value_1:
	.double 2.5
value_2:
	.double 3.14

	.text
	.globl	main
main:
	pushq	%rbp 		# save caller's base pointer
	movq 	%rsp, %rbp 	# store stack pointer in current base pointer
	subq	$48, %rsp	# save 32 shadow space for printf, + 8 bytes for local float variable + 8 bytes for alignment = 48
                        # Alignment notes: 
						# 48 bytes + 8 bytes from push base point + 8 bytes from call printf = 64 
                        # which is divisible by 16, and complies with 16 byte boundary  

	movsd	value_1(%rip), %xmm0 	# load first value into float register
	movsd	value_2(%rip), %xmm1 	# load second value into float register
	
	addsd %xmm0, %xmm1 # add two floats together xmm1 = xmm0 + xmm1

	movsd	%xmm1, -8(%rbp)		# load result of float addition into memory

	leaq	msg(%rip), %rcx		# first arg for printf
	movq	-8(%rbp), %rdx		# second arg for printf

	call	printf

	movl	$0, %eax	# exit code
	addq	$48, %rsp	# restore stack
	popq	%rbp		# restore caller's base pointer
	ret
