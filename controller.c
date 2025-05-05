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

void testCompiler(char* input, int expectedResult, int fileIndex) {
    int result;

    char* compiled = compiler(input);

    writeCompiledToFile(compiled, fileIndex);

    // Assemble compiled assembly
    char* buffer = malloc(100);
    sprintf(buffer, "gcc -o assembled_machine_code.exe compiled_assembly-%d.s", fileIndex);
    system(buffer);
    free(buffer);

    // Run assembled machine code
    result = system("assembled_machine_code.exe");

    printf("Output from running assembled machine code: %d\n\n", result);

    if (result == expectedResult) {
        printf("Test Passed!\n");
    } else {
        printf("Test failed, expected result %d, actual result %d\n", expectedResult, result);
    }
}

int main() {
    testCompiler("11+5", 16, 1);
    testCompiler("10/5+3*2*3-11", 9, 2);
    testCompiler("10/5+3*2-11+5", 2, 3);
    testCompiler("10 / 5 +3*  2-11 + 5", 2, 4);
    testCompiler(" 1 + 4 ", 5, 5);
   
    return 0;
}