.section .data
msg:
    .string "Hello, World!\n"

.section .text
.extern printf
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp

    subq $32, %rsp            # align stack to 16 bytes

    leaq msg(%rip), %rcx      # Windows x64 ABI: RCX = first argument
    call printf

    addq $32, %rsp            # restore stack

    movl $0, %eax
    popq %rbp
    ret
