#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* compiled;

int initialCode() {
    strcpy(compiled,
        ".section .text\n"
        ".global main\n\n"
        "main:\n"
    );
    printf(compiled);
}

int addToCompiled (const char *code) {
    printf(code);
    strcat(compiled, code);
}

int finalCode() {
    // Output top of stack to exit code
    strcat(compiled,
        "\n"
        "\t# Final code follows, this takes value from top of stack and places in exit code\n"
        "\t# Note only the 32 lowest bits of RAX will be used in exit code\n"
        "\n"
        "\tpop %%rax\n"
        "\tret\t# Return from main\n"
    );
}

int writeCompiledToFile () {
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
    compiled = malloc(1000);

    printf("\nOutput from compiler follows:\n\n");

    initialCode();

    // main code
    addToCompiled("\tpush $2\n");

    finalCode();

    writeCompiledToFile();

    return 0;
}