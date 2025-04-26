#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* input;
char* compiled;
char currentChar;
int pos;

void initialCode() {
    strcpy(compiled,
        ".section .text\n"
        ".global main\n\n"
        "main:\n"
    );
    printf(compiled);
}

void addToCompiled (const char *code) {
    printf(code);
    strcat(compiled, code);
}

void nextChar() {
    currentChar = input[pos++];
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Parse a number and emit a push instruction
void parseNumber() {
    int value = 0;
    while (isDigit(currentChar)) {
        value = value * 10 + (currentChar - '0');
        nextChar();
    }

    char* buffer = malloc(20);
    sprintf(buffer, "\tpush $%d\n", value);
    addToCompiled(buffer);
    free(buffer);
}

void parseExpression () {
    if (isDigit(currentChar)) {
        parseNumber();
    }

    if (currentChar == '+') {
        nextChar();
        parseExpression();
        
        addToCompiled(
            "\n"
            "\tpop %%rax\n"
            "\tpop %%rbx\n"
            "\tadd %%rbx, %%rax\n"
            "\tpush %%rax\n"
            "\n"
        );
    }

    return;
}

void compileInput() {
    pos = 0;

    nextChar();

    parseExpression();

    return;
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
    input = malloc(100);

    printf("\nOutput from compiler follows:\n\n");

    initialCode();

    // set input to compiler
    input = "23+77";

    // generate main code
    compileInput();

    finalCode();

    writeCompiledToFile();

    return 0;
}