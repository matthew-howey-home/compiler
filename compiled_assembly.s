.section .text
.global main

main:
	# Evaluating: 10/5+3*2*3-10

	push $10
	push $5

	pop %rbx			# right hand operand
	pop %rax			# left hand operand
	cqo				# convert quad rax to oct rdx:rax
	idiv %rbx		# rax = rdx:rax / rbx
	push %rax

	push $3
	push $2

	pop %rbx			# right hand operand
	pop %rax			# left hand operand
	imul %rbx, %rax		# rax = rax * rbx
	push %rax

	push $3

	pop %rbx			# right hand operand
	pop %rax			# left hand operand
	imul %rbx, %rax		# rax = rax * rbx
	push %rax


	pop %rbx			# right hand operand
	pop %rax			# left hand operand
	add %rbx, %rax		# rax = rax + rbx
	push %rax

	push $10

	pop %rbx			# right hand operand
	pop %rax			# left hand operand
	sub %rbx, %rax		# rax = rax - rbx
	push %rax


	# Final code follows, this takes value from top of stack and places in exit code
	# Note only the 32 lowest bits of RAX will be used in exit code

	pop %rax
	ret	# Return from main
