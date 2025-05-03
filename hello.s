# Simple hello world
# To compile, enter: gcc hello.s -o hello.exe -lmsvcrt
# To run, enter: ./hello.exe

.section .data
msg:
    .string "Hello, World!\n"

.section .text
.extern printf
.globl main
main:
    subq $40, %rsp            # save 32 shadow space + 8 for 64bit alignment

    leaq msg(%rip), %rcx      # Windows x64 ABI: RCX = first argument
    call printf

    addq $32, %rsp            # restore stack

    movl $0, %eax
    ret
