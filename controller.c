#include <stdio.h>
#include <stdlib.h>

int main() {
    int result;

    // Compile and run compiler
    system("gcc -o compiler.exe compiler.c");
    system("compiler.exe");

    // Assemble compiled assembly
    system("gcc -o assembled_machine_code.exe compiled_assembly.s");

    // Run assembled machine code
    result = system("assembled_machine_code.exe");

    printf("Output from running assembled maching code: %d\n", result);
    return 0;
}