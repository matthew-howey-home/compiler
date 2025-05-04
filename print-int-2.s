# Simple printf of int, will save the int to a file when complete
# To compile, enter: gcc print-int-2.s -o print-int-2.exe -lmsvcrt
# To run, enter: ./print-int-2.exe

.section .data
msg:
    .string "The number is %d!\n"
buffer:
    .string

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

    addq $40, %rsp            # restore stack

    movl $0, %eax
    ret
