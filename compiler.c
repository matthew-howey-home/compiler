#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct globalCtx {
    char* input;
    char* compiled;
};

char currentChar;
int pos;

void initialCode(struct globalCtx* ctx) {
    strcpy(ctx->compiled,
        ".section .text\n"
        ".global main\n\n"
        "main:\n"
    );
    printf(ctx->compiled);
}

void addToCompiled (struct globalCtx* ctx, const char *code) {
    printf(code);
    strcat(ctx->compiled, code);
}

void nextChar(struct globalCtx* ctx)
{
    currentChar = ctx->input[pos++];
}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Parse a number and emit a push instruction
void parseNumber(struct globalCtx* ctx) {
    int value = 0;
    while (isDigit(currentChar)) {
        value = value * 10 + (currentChar - '0');
        nextChar(ctx);
    }

    char* buffer = malloc(20);
    sprintf(buffer, "\tpush $%d\n", value);
    addToCompiled(ctx, buffer);
    free(buffer);
}


void parseFactor(struct globalCtx* ctx) {
    if (isDigit(currentChar)) {
        parseNumber(ctx);  // emits "push number"
    } else {
        // You could add support for parentheses here later if you want!
    }
}

void parseTerm(struct globalCtx* ctx) {
    parseFactor(ctx);
    while (currentChar == '*' || currentChar == '/') {
        char operator = currentChar;
        nextChar(ctx);
        parseFactor(ctx);
        addToCompiled(ctx,
            "\n"
            "\tpop %%rbx\t\t\t# right hand operand\n"
            "\tpop %%rax\t\t\t# left hand operand\n"
        );
        if (operator == '*') {
            addToCompiled(ctx, "\timul %%rbx, %%rax\t\t# rax = rax * rbx\n");
        } else {
            addToCompiled(ctx,
                "\tcqo\t\t\t\t# convert quad rax to oct rdx:rax\n"
                "\tidiv %%rbx\t\t# rax = rdx:rax / rbx\n"
            );
        }
        addToCompiled(ctx, "\tpush %%rax\n\n");
    }
}

void parseExpression(struct globalCtx* ctx) {
    parseTerm(ctx);
    while (currentChar == '+' || currentChar == '-') {
        char operator = currentChar;
        nextChar(ctx);
        parseTerm(ctx);
        addToCompiled(ctx,
            "\n"
            "\tpop %%rbx\t\t\t# right hand operand\n"
            "\tpop %%rax\t\t\t# left hand operand\n"
        );
        if (operator == '+') {
            addToCompiled(ctx, "\tadd %%rbx, %%rax\t\t# rax = rax + rbx\n");
        } else {
            // sub %rbx, %rax	rax = rax - rbx
            addToCompiled(ctx, "\tsub %%rbx, %%rax\t\t# rax = rax - rbx\n");
        }
        addToCompiled(ctx,
            "\tpush %%rax\n\n"
        );
    }
}

void compileInput(struct globalCtx* ctx) {
    pos = 0;

    nextChar(ctx);

    parseExpression(ctx);

    return;
}

int finalCode(struct globalCtx* ctx) {
    // Output top of stack to exit code
    strcat(ctx->compiled,
        "\n"
        "\t# Final code follows, this takes value from top of stack and places in exit code\n"
        "\t# Note only the 32 lowest bits of RAX will be used in exit code\n"
        "\n"
        "\tpop %%rax\n"
        "\tret\t# Return from main\n"
    );
}


void compiler(struct globalCtx* ctx) {
    printf("\nOutput from compiler follows:\n\n");

    initialCode(ctx);


    addToCompiled(ctx, "\t# Evaluating: ");
    addToCompiled(ctx, ctx->input);
    addToCompiled(ctx, "\n\n");

    // generate main code
    compileInput(ctx);

    finalCode(ctx);
}