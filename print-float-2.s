
.section .data

msg:
	.string "The number is %f!"
value:
	.double 3.1452

	.text
	.globl	main
main:
	pushq	%rbp 		# save caller's base pointer
	movq 	%rsp, %rbp 	# store stack pointer in current base pointer
	subq	$32, %rsp	# save 32 shadow space
                        # 32 bytes + 8 bytes from push base point + 8 bytes from call printf 
                        # which is divisible by 16, and complies with 16 byte boundary  

	movsd	value(%rip), %xmm0 	# load value into float register
	movsd	%xmm0, -8(%rbp)		# load float register into memory

	leaq	msg(%rip), %rcx		# first arg for printf
	movq	-8(%rbp), %rdx		# second arg for printf

	call	printf

	movl	$0, %eax	# exit code
	addq	$32, %rsp	# restore stack
	popq	%rbp		# restore caller's base pointer
	ret
