	.file	"print-float.c"
	.text
	.section .rdata,"dr"
.LC0:
	.ascii "Value: %f\0"
	.text
	.globl	main
main:
	subq	$40, %rsp
	movabsq	$4614253070214989087, %rax
	movq	%rax, %xmm0
	movapd	%xmm0, %xmm1
	movq	%rax, %rdx
	leaq	.LC0(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, %eax
	addq	$40, %rsp
	ret
	
