.section .text
.global main

main:
	# Evaluating:  1 + 4 

	push $1
	push $4

	pop %rbx			# right hand operand
	pop %rax			# left hand operand
	add %rbx, %rax		# rax = rax + rbx
	push %rax


	# Final code follows, this takes value from top of stack and places in exit code
	# Note only the 32 lowest bits of RAX will be used in exit code

	pop %rax
	ret	# Return from main
