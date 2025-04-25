#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* parsed;
char* input;
char currentChar;
int pos;

// Function to advance one character
void nextChar() {
    currentChar = input[pos++];
}

// Check if current char is a digit
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
    sprintf(buffer, "    push %d\n", value);
    strcpy(parsed, buffer);
}

char* parser() {
    // Just a test: copy input into parsed
    parsed = malloc(100);
    pos = 0;

    nextChar(); 

    if (isDigit(currentChar)) {
        parseNumber();
    }

    return parsed;
}

int main() {
    input = malloc(100);
    if (input == NULL) return 1;

    strcpy(input, "2");

    char* result = parser();
    printf("Parsed result: %s\n", result);

    free(input);
    free(parsed);

    return 0;
}