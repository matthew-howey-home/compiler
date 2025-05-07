# Simple output to file
# To compile, enter: gcc output-to-file.s -o output-to-file.exe -lmsvcrt
# To run, enter: ./output-to-file.exe

.section .data

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

    leaq file_name(%rip), %rcx   # load filename into first arg
    leaq method(%rip), %rdx     # load method w (write) into second arg
    call fopen
    movq %rax, %r12              # save FILE* returned by fopen

    leaq file_content(%rip), %rcx      # pointer to file content
    movq $1, %rdx                # size
    movq $5, %r8                # count
    movq %r12, %r9               # FILE*
    call fwrite

    movq %r12, %rcx             # load file reference into first arg
    call fclose

    addq $40, %rsp            # restore stack

    movl $0, %eax
    ret
