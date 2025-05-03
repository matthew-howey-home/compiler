	.file	"print-float-2.c"
	.text
	.section .rdata,"dr"
.LC1:
	.ascii "Value: %f\12\0"
	.text
	.globl	main
main:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$48, %rsp
	call	__main
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -8(%rbp)
	movsd	-8(%rbp), %xmm0
	movq	-8(%rbp), %rax
	movapd	%xmm0, %xmm1
	movq	%rax, %rdx
	leaq	.LC1(%rip), %rax
	movq	%rax, %rcx
	call	printf
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret

	.section .rdata,"dr"
	.align 8
.LC0:
	.long	0
	.long	1075314688
	