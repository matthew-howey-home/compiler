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

int floatIndex = 0;

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
        ".section .data\n\n"
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
    char* buffer = malloc(100);
    sprintf(buffer, "\tpush $%s\n", numberAsString);
    addToCompiled(buffer);
    free(buffer);
}

void pushFloat(char* numberAsString) {
    char* buffer = malloc(100);
    sprintf(buffer, "float_var_%d: .double %s\n", floatIndex, numberAsString);
    addToDataSection(buffer);

    addToCompiled("\tsub $8, %%rsp\t\t\t# Push float (as double) to stack\n");
    buffer = malloc(100);
    sprintf(buffer, "\tmovsd float_var_%d(%%%%rip), %%%%xmm0\n", floatIndex);
    addToCompiled(buffer);
    addToCompiled("\tmovsd %%xmm0, (%%rsp)\n\n");

    free(buffer);
    floatIndex++;
}

// Parse a number and emit a push instruction
enum DataType parseNumber() {
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
    } else if (dataType == FLOAT) {
        pushFloat(numberAsString);
    }
    free(numberAsString);

    return dataType;
}

enum DataType parseExpression();

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

enum DataType parseFactor() {
    enum DataType dataType;
    skipWhitespace();
    if (isDigit(currentChar)) {
        dataType = parseNumber();  // emits "push number"
    } else if (currentChar == '(') {
        parseFactorInBrackets();
    }
    return dataType;
}

enum DataType parseTerm() {
    enum DataType dataType;
    dataType = parseFactor();

    skipWhitespace();
    while (currentChar == '*' || currentChar == '/') {
        char operator = currentChar;
        nextChar();
        parseFactor();
        addToCompiled(
            "\n"
            "\tpop %%rbx\t\t\t# pop right hand operand int from stack into rbx\n"
            "\tpop %%rax\t\t\t# pop left hand operand int from stack into rax\n"
        );
        if (operator == '*') {
            addToCompiled("\timul %%rbx, %%rax\t\t# operation: rax = rax * rbx\n");
        } else {
            addToCompiled(
                "\tcqo\t\t\t\t\t# convert quad rax to octal rdx:rax\n"
                "\tidiv %%rbx\t\t\t# operation: rax = rdx:rax / rbx\n"
            );
        }
        addToCompiled("\tpush %%rax\t\t\t# save result of operation on stack\n\n");
    }
    return dataType;
}

enum DataType parseExpression() {
    enum DataType leftOperandDataType = parseTerm();
    enum DataType dataType = leftOperandDataType;

    skipWhitespace();
    while (currentChar == '+' || currentChar == '-') {
        char operator = currentChar;
        nextChar();
        enum DataType rightOperandDataType = parseTerm();

        if (leftOperandDataType == FLOAT || rightOperandDataType == FLOAT) {
            dataType = FLOAT;
        }

        if (dataType == INT) {
            addToCompiled(
                "\n"
                "\tpop %%rbx\t\t\t# pop right hand operand int from stack into rbx\n"
                "\tpop %%rax\t\t\t# pop left hand operand int from stack into rax\n\n"
            );
            if (operator == '+') {
                addToCompiled("\n\tadd %%rbx, %%rax\t\t# int operation: rax = rax + rbx\n");
            } else if (operator == '-') {
                addToCompiled("\n\tsub %%rbx, %%rax\t\t# int operation: rax = rax - rbx\n");
            }
            addToCompiled("\tpush %%rax\t\t\t\t# save result of int operation to stack\n\n");
        } else if (dataType == FLOAT) {
            if (rightOperandDataType == FLOAT) {
                addToCompiled(
                    "\n"
                    "\tmovsd (%%rsp), %%xmm1\t\t# pop right hand operand float from stack into xmm1\n"
                    "\tadd $8, %%rsp\n"
                );
            } else if (rightOperandDataType == INT) {
                addToCompiled(
                    "\n"
                    "\tpop %%rax\t\t\t\t# pop right hand operand int from stack into rax\n"
                    "\tcvtsi2sd %%rax, %%xmm1\t# Convert int rax to float in xmm1\n"
                );
            }
            if (leftOperandDataType == FLOAT) {
                addToCompiled(
                    "\n"
                    "\tmovsd (%%rsp), %%xmm0\t\t# pop left hand operand float from stack into xmm0\n"
                    "\tadd $8, %%rsp\n"
                );
            } else if (leftOperandDataType == INT) {
                addToCompiled(
                    "\n"
                    "\tpop %%rax\t\t\t\t# pop right hand operand int from stack into rax\n"
                    "\tcvtsi2sd %%rax, %%xmm0\t# Convert int rax to float in xmm0\n"
                );
            }
            if (operator == '+') {
                addToCompiled("\n\taddsd %%xmm1, %%xmm0\t\t# float operation: xmm0 = xmm0 + xmm1\n");
            } else  if (operator == '-') {
                addToCompiled("\n\tsubsd %%xmm1, %%xmm0\t\t# float operation: xmm0 = xmm0 - xmm1\n");
            }
            addToCompiled(
                "\n"
                "\tsub $8, %%rsp\t\t\t# save result of float operation to stack\n"
                "\tmovsd %%xmm0, (%%rsp)\n\n"
            );
            leftOperandDataType = dataType;
        }

    }
    return dataType;
}

enum DataType compileInput() {
    pos = 0;

    nextChar();

    enum DataType dataType = parseExpression();

    return dataType;
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

void finalCode(enum DataType dataType) {
    char* datasection;
    if (dataType == INT) {
        datasection = loadFile("fragment-sectiondata-int-out.s");
    } else if (dataType == FLOAT) {
         datasection = loadFile("fragment-sectiondata-float-out.s");
    }
    addToDataSection(datasection);
    free(datasection);
    datasection = NULL;

    char* main;
    main = loadFile("fragment-main-out.s");
    
    addToCompiled(main);
    free(main);
    main = NULL; 
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
    enum DataType dataType = compileInput();

    finalCode(dataType);

    char* output = malloc(5000);

    strcpy(output, ctx.datasection);
    strcat(output, "\n");
    strcat(output, ctx.compiled);

    return output;
}