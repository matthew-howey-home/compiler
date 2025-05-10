.section .data

float_var_7: .double 2.0

######################## Frgament of code to write the result of evaluation to a file output.txt, fomratted as integer (data section)
number_format:
    .string "%f"
buffer:
    .space 200

file_name:
    .string "output.txt"
method:
    .string "w"
####################################################################################

.section .text
.global main

main:
	# Evaluating: 1+2.0+3

	push $1
	sub $8, %rsp					# Make space on stack for 8 bytes
	movsd float_var_7(%rip), %xmm0
	movsd %xmm0, (%rsp)				# Store 8 bytes (double) on stack


	movsd (%rsp), %xmm1		# Load 8-byte float from the top of the stack into xmm1
	add $8, %rsp		# Adjust the stack pointer (pop 8 bytes)

	pop %rax
	cvtsi2sd %rax, %xmm0		#Convert int to float

	addsd %xmm1, %xmm0		# xmm0 += xmm1

	sub $8, %rsp					# Make space on stack for 8 bytes
	movsd %xmm0, (%rsp)				# Store 8 bytes (double) on stack

	push $3

	pop %rax
	cvtsi2sd %rax, %xmm1		#Convert int to float

	movsd (%rsp), %xmm0		# Load 8-byte float from the top of the stack into xmm0
	add $8, %rsp		# Adjust the stack pointer (pop 8 bytes)

	addsd %xmm1, %xmm0		# xmm0 += xmm1

	sub $8, %rsp					# Make space on stack for 8 bytes
	movsd %xmm0, (%rsp)				# Store 8 bytes (double) on stack

######################## Fragment of code to write the result of evaluation to a file output.txt, formatted as integer (main code)
    popq %rax          # save result from evaluation to rax

    subq $40, %rsp     # save 32 shadow space + 8 for 64bit alignment
                        # 40 bytes + 8 bytes from printf = 48
                        # which is divisible by 16, and complies with 16 byte boundary  

    # Store value in string buffer, formatted as per number_format: sprintf(buffer, number_format, RAX)
    leaq buffer(%rip), %rcx           # load buffer into first arg
    leaq number_format(%rip), %rdx    # load number_format into second arg
    movq %rax, %r8                    # load result of evaluation into third arg
    call sprintf

    # Open file: FILE* r12 = fopen(file_name, method)
    leaq file_name(%rip), %rcx        # load filename into first arg
    leaq method(%rip), %rdx           # load method w (write) into second arg
    call fopen
    movq %rax, %r12                   # save FILE* returned by fopen

    # compute length of string in buffer, rcx = strlen(buffer)
    leaq buffer(%rip), %rcx   # first arg to strlen: pointer to buffer
    call strlen                 # returns length in RAX

    # write buffer to file: fwrite(buffer, 1, rax, r12)
    leaq buffer(%rip), %rcx       # first arg - pointer to file content
    movq $1, %rdx                  # second arg - size
    movq %rax, %r8                # third arg - count (output from strlen) (total size of file is size * count)
    movq %r12, %r9                # FILE*
    call fwrite

    # close file: fclose(r12)
    movq %r12, %rcx             # load file reference into first arg
    call fclose

######################## end function main

    addq $40, %rsp            # restore stack

    movl $0, %eax
    ret
