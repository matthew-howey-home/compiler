	.file	"print-float.c"
	.text
	.section .rdata,"dr"
.LC0:
	.ascii "Value: %f\0"
	.text
	.globl	main
main:
	subq	$40, %rsp
	movabsq	$4614253070214989087, %rdx
	leaq	.LC0(%rip), %rcx
	call	printf
	movl	$0, %eax
	addq	$40, %rsp
	ret
	
