#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct globalCtx {
    char* input;
    char* compiled;
    char* datasection;
};

struct globalCtx ctx;

char currentChar;
int pos;

enum DataType {
    INT, FLOAT
};

void initialCode() {
    strcpy(ctx.compiled,
        ".section .text\n"
        ".global main\n\n"
        "main:\n"
    );
    strcpy(ctx.datasection,
        ".section .data\n"
    );
}

void addToCompiled (const char *code) {
    strcat(ctx.compiled, code);
}

void addToDataSection(const char* code) {
    strcat(ctx.datasection, code);
}

void nextChar()
{
    currentChar = ctx.input[pos++];
}

void skipWhitespace() {
    while(currentChar == ' ') {
        nextChar();
    }    
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

void pushInt(char* numberAsString) {
    char* buffer = malloc(30);
    sprintf(buffer, "\tpush $%s\n", numberAsString);
    addToCompiled(buffer);
    free(buffer);
}

// Parse a number and emit a push instruction
void parseNumber() {
    enum DataType dataType = INT;
 
    char* numberAsString = malloc(20);
    int numberAsStringPos = 0;
    while (isDigit(currentChar) || currentChar == '.') {
        numberAsString[numberAsStringPos++] = currentChar;
        if (currentChar == '.') { dataType = FLOAT; }
        nextChar();
    }
    numberAsString[numberAsStringPos] = '\0';

    if (dataType == INT) {
        pushInt(numberAsString);
    }
    free(numberAsString);
}

void parseExpression();

void parseFactorInBrackets() {
    nextChar();
    parseExpression();
    skipWhitespace();
    if (currentChar == ')') {
        nextChar();
    } else {
        printf("Error: expected )");
        exit(1);
    }
}

void parseFactor() {
    skipWhitespace();
    if (isDigit(currentChar)) {
        parseNumber();  // emits "push number"
    } else if (currentChar == '(') {
        parseFactorInBrackets();
    }
}

void parseTerm() {
    parseFactor();

    skipWhitespace();
    while (currentChar == '*' || currentChar == '/') {
        char operator = currentChar;
        nextChar();
        parseFactor();
        addToCompiled(
            "\n"
            "\tpop %%rbx\t\t\t# right hand operand\n"
            "\tpop %%rax\t\t\t# left hand operand\n"
        );
        if (operator == '*') {
            addToCompiled("\timul %%rbx, %%rax\t\t# rax = rax * rbx\n");
        } else {
            addToCompiled(
                "\tcqo\t\t\t\t# convert quad rax to oct rdx:rax\n"
                "\tidiv %%rbx\t\t# rax = rdx:rax / rbx\n"
            );
        }
        addToCompiled("\tpush %%rax\n\n");
    }
}

void parseExpression() {
    parseTerm();

    skipWhitespace();
    while (currentChar == '+' || currentChar == '-') {
        char operator = currentChar;
        nextChar();
        parseTerm();
        addToCompiled(
            "\n"
            "\tpop %%rbx\t\t\t# right hand operand\n"
            "\tpop %%rax\t\t\t# left hand operand\n"
        );
        if (operator == '+') {
            addToCompiled("\tadd %%rbx, %%rax\t\t# rax = rax + rbx\n");
        } else {
            // sub %rbx, %rax	rax = rax - rbx
            addToCompiled("\tsub %%rbx, %%rax\t\t# rax = rax - rbx\n");
        }
        addToCompiled(
            "\tpush %%rax\n\n"
        );
    }
}

void compileInput() {
    pos = 0;

    nextChar();

    parseExpression();

    return;
}

char* loadFile(char* filename) {
    FILE* file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    char* output = malloc(fileSize + 1);
    rewind(file);
    size_t  bytesRead = fread(output, 1, fileSize, file);
    output[bytesRead] = '\0'; // null-terminate the string
    fclose(file);

    return output;
}

void finalCode() {
    char* datasection = loadFile("fragment-sectiondata-int-out.s");
    addToDataSection(datasection);
    free(datasection);
    datasection = NULL;

    char* mainIntout = loadFile("fragment-main-int-out.s");
    addToCompiled(mainIntout);
    free(mainIntout);
    mainIntout = NULL; 
}


char* compiler(char* input) {
    // printf("\nOutput from compiler follows:\n\n");

    ctx.compiled = malloc(5000);
    ctx.datasection = malloc(5000);
    ctx.input = input;
    initialCode();

    addToCompiled("\t# Evaluating: ");
    addToCompiled(ctx.input);
    addToCompiled("\n\n");

    // generate main code
    compileInput();

    finalCode();

    char* output = malloc(5000);

    strcpy(output, ctx.datasection);
    strcat(output, "\n");
    strcat(output, ctx.compiled);

    return output;
}