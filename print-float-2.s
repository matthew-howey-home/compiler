	.file	"print-float-2.c"
	.text
	.section .rdata,"dr"
.LC1:
	.ascii "Value: %f\12\0"
	.text
	.globl	main
main:
	subq	$40, %rsp

	movsd	.LC01(%rip), %xmm0
	movsd	%xmm0, -8(%rbp)
	movq	-8(%rbp), %rdx
	leaq	.LC1(%rip), %rcx
	call	printf

	movl	$0, %eax
	addq	$40, %rsp
	ret

	.section .rdata,"dr"
	.align 8
.LC01:
    .double 3.1452
