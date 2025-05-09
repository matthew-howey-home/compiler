#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaration of the compiler function
char* compiler(char* input);

int writeCompiledToFile (char* compiled, int fileIndex) {
    char* filename = malloc(20);
    sprintf(filename, "compiled_assembly-%d.s", fileIndex);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }
    fprintf(file, compiled);
    fclose(file);
    printf("\nCompiled assembly code written to file %s\n\n", filename);

    free(filename);
}

char* getOutputFromFile() {
    FILE* file = fopen("output.txt", "r");
    char* buffer = malloc(100);
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    return buffer;
}

void testCompiler(char* input, int expectedResult, int fileIndex) {
    char* compiled = compiler(input);

    writeCompiledToFile(compiled, fileIndex);

    // Assemble compiled assembly
    char* buffer = malloc(100);
    sprintf(buffer, "gcc -o assembled_machine_code.exe compiled_assembly-%d.s", fileIndex);
    system(buffer);
    free(buffer);

    // Run assembled machine code
    system("assembled_machine_code.exe");

    char* output = getOutputFromFile();

    char *endptr;
    int result = strtol(output, &endptr, 10);  // base 10

    printf("Output from running assembled machine code: %s\n\n", buffer);

    if (result == expectedResult) {
        printf("Test Passed!\n");
    } else {
        printf("****************OH NO*********!!!!! Test failed,\n expected result %d, actual result %d\n", expectedResult, result);
    }
}

int main() {
    testCompiler("11+5", 16, 1);
    testCompiler("10/5+3*2*3-11", 9, 2);
    testCompiler("10/5+3*2-11+5", 2, 3);
    testCompiler("10 / 5 +3*  2-11 + 5", 2, 4);
    testCompiler(" 1 + 4 ", 5, 5);
    testCompiler("2*(3+8)", 22, 6);
    testCompiler(" 2 * (11 - 2 ) ", 18, 7);
    // testCompiler("2.0", 2.0, 8);
    testCompiler("23647", 23647, 9);
   
    return 0;
}