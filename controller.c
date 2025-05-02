#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct globalCtx {
    char* input;
    char* compiled;
};

struct globalCtx ctx;

// Declaration of the compiler function
void compiler(struct globalCtx* ctx);

int writeCompiledToFile (char* compiled) {
    FILE *file = fopen("compiled_assembly.s", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }
    fprintf(file, compiled);
    fclose(file);
    printf("\nCompiled assembly code written to file compiled_assembly.s\n\n");
}

void testCompiler(char* input, int expectedResult) {
    int result;
    ctx.input = malloc(100);
    strcpy(ctx.input, input);

    compiler(&ctx);

    writeCompiledToFile(ctx.compiled);

    // Assemble compiled assembly
    system("gcc -o assembled_machine_code.exe compiled_assembly.s");

    // Run assembled machine code
    result = system("assembled_machine_code.exe");

    printf("Output from running assembled machine code: %d\n\n", result);

    if (result == expectedResult) {
        printf("Test Passed!");
    } else {
        printf("Test failed, expected result %d, actual result %d", expectedResult, result);
    }
}

int main() {
    ctx.compiled = malloc(1000);

    testCompiler("10/5+3*2*3-11", 9);
    testCompiler("10/5+3*2-11+5", 2);
   
    return 0;
}