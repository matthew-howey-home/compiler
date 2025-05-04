	.file	"print-float-2.c"
	.text
	.section .rdata,"dr"
.LC1:
	.ascii "Value: %f\12\0"
	.text
	.globl	main
main:
	pushq	%rbp 		# save caller's base pointer
	movq 	%rsp, %rbp 	# store stack pointer in current base pointer
	subq	$32, %rsp	# save 32 shadow space
                        # 32 bytes + 8 bytes from push base point + 8 bytes from call printf 
                        # which is divisible by 16, and complies with 16 byte boundary  

	movsd	.LC01(%rip), %xmm0
	movsd	%xmm0, -8(%rbp)
	movq	-8(%rbp), %rdx
	leaq	.LC1(%rip), %rcx
	call	printf

	movl	$0, %eax	# exit code
	addq	$32, %rsp	# restore stack
	popq	%rbp		# restore caller's base pointer
	ret

	.section .rdata,"dr"
	.align 8
.LC01:
    .double 3.1452
