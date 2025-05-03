# Simple hello world
# To compile, enter: gcc hello.s -o hello.exe -lmsvcrt
# To run, enter: ./hello.exe

.section .data
msg:
    .string "The number is %d!\n"

.section .text
.extern printf
.globl main
main:
    subq $40, %rsp            # save 32 shadow space + 8 for 64bit alignment

    leaq msg(%rip), %rcx      # Windows x64 ABI: RCX = first argument
    movl $53, %edx            # load int32 53 = second argument 
    call printf

    addq $40, %rsp            # restore stack

    movl $0, %eax
    ret
