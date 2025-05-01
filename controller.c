#include <stdio.h>
#include <stdlib.h>

struct globalCtx {
    char* input;
    char* compiled;
};

// Declaration of the compiler function
void compiler(struct globalCtx ctx);

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

int main() {
    struct globalCtx ctx;
    char* compiled = malloc(1000);
    char* input = malloc(100);
    ctx.compiled = compiled;
    ctx.input = input;

    int result;

    // set input to compiler
    ctx.input = "10/5+3*2*3-10";

    // Compile and run compiler
    printf("####################\n");
    printf("Running compiler\n");
    printf("####################\n");
    compiler(ctx);

    writeCompiledToFile(ctx.compiled);

    // Assemble compiled assembly
    system("gcc -o assembled_machine_code.exe compiled_assembly.s");

    // Run assembled machine code
    result = system("assembled_machine_code.exe");

    printf("Output from running assembled machine code: %d\n\n", result);
    return 0;
}