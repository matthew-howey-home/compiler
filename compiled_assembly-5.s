.section .data


######################## Frgament of code to write the result of evaluation to a file output.txt, fomratted as integer (data section)
number_format:
    .string "%d"
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
	# Evaluating:  1 + 4 

	push $1		# Push int to stack

	push $4		# Push int to stack


	pop %rbx			# pop right hand operand int from stack into rbx
	pop %rax			# pop left hand operand int from stack into rax


	add %rbx, %rax		# int operation: rax = rax + rbx
	push %rax				# save result of int operation to stack

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
