.section .text
.global main

main:
	# Evaluating: 10 / 5 +3*  2-11 5

	push $10

	# Final code follows, this takes value from top of stack and places in exit code
	# Note only the 32 lowest bits of RAX will be used in exit code

	pop %rax
	ret	# Return from main
