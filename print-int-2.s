# Simple printf of int, will save the int to a file when complete
# To compile, enter: gcc print-int-2.s -o print-int-2.exe -lmsvcrt
# To run, enter: ./print-int-2.exe

.section .data
msg:
    .string "The number is %d!\n"
buffer:
    .space 200

file_name:
    .string "output.txt"
method:
    .string "w"
file_content:
    .string "Hello"

.section .text
.globl main
main:
    subq $40, %rsp            # save 32 shadow space + 8 for 64bit alignment
                              # 40 bytes + 8 bytes from printf = 48
                              # which is divisible by 16, and complies with 16 byte boundary  

    leaq buffer(%rip), %rcx   # load buffer into first arg
    leaq msg(%rip), %rdx      # load msg into second arg
    movq $53, %r8            # load int64 53 into third arg
    call sprintf

    leaq buffer(%rip), %rcx # load buffer into first arg
    call printf

######################## Write to file sample code - as next step this needs to write the buffer to the file

    leaq file_name(%rip), %rcx   # load filename into first arg
    leaq method(%rip), %rdx     # load method w (write) into second arg
    call fopen
    movq %rax, %r12              # save FILE* returned by fopen

    leaq file_content(%rip), %rcx      # first arg - pointer to file content
    movq $1, %rdx                # second arg - size
    movq $5, %r8                # third arg - count (total size of file is size * count)
    movq %r12, %r9               # FILE*
    call fwrite

    movq %r12, %rcx             # load file reference into first arg
    call fclose


######################## end write to file sample code

    addq $40, %rsp            # restore stack

    movl $0, %eax
    ret
