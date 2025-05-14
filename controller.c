#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaration of the compiler function
char* compiler(char* input);

int failedTests = 0;

int writeCompiledToFile (char* compiled, int fileIndex) {
    char* filename = malloc(50);
    sprintf(filename, "./compiled-files/compiled_assembly-%d.s", fileIndex);

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

void testCompiler(char* input, double expectedResult, int fileIndex) {
    char* compiled = compiler(input);

    writeCompiledToFile(compiled, fileIndex);

    // Assemble compiled assembly
    char* buffer = malloc(100);
    sprintf(buffer, "gcc -o assembled_machine_code.exe compiled-files\\compiled_assembly-%d.s", fileIndex);
    system(buffer);
    free(buffer);

    // Run assembled machine code
    system("assembled_machine_code.exe");

    char* output = getOutputFromFile();
    printf("Output from running assembled machine code: %s\n\n", output);

    char *endptr;
    double result = strtod(output, &endptr);
    free(output);

    if (result == expectedResult) {
        printf("Test Passed!\n");
    } else {
        printf("****************OH NO*********!!!!! Test failed,\n expected result %f, actual result %f\n", expectedResult, result);
        failedTests++;
    }
}

int main() {
    testCompiler("23647", 23647, 0);
    testCompiler("11+5", 16, 1);
    testCompiler("10/5+3*2*3-11", 9, 2);
    testCompiler("10/5+3*2-11+5", 2, 3);
    testCompiler("10 / 5 +3*  2-11 + 5", 2, 4);
    testCompiler(" 1 + 4 ", 5, 5);
    testCompiler("2*(3+8)", 22, 6);
    testCompiler(" 2 * (11 - 2 ) ", 18, 7);
   
    testCompiler("2.0", 2, 8);
    testCompiler("2.0+1.0", 3, 9);
    testCompiler("2.0+1.0+3.0", 6, 9);

    testCompiler("1+2.0", 3, 10);
    testCompiler("1+2.0+3", 6, 11);
    testCompiler("2.5 + 2.5", 5, 12);
    testCompiler("5.0 - 2", 3, 13);
    testCompiler("2 + 3.0 - 5 + 6.0", 6, 14);
    testCompiler("2 + 2.5", 4.5, 15);
    testCompiler("3.5 - 2.2 + 8.1", 9.4, 16);

    testCompiler("2.0 * 3.0", 6, 17);
    testCompiler("2 * 3.4", 6.8, 18);
    testCompiler("3 + 4.5 * 2 - 8", 4, 19);

    testCompiler(" 3.0 / 2", 1.5, 20);
    testCompiler("1 / 3.0 * 3", 1, 21);
    testCompiler("1 / 3 * 3.0", 0, 22);
    testCompiler("1 * (2 + 3.0) ", 5, 23);

    testCompiler("1 * 4 / 2 +6.6 - 9 / (1 + 2)", 5.6, 24);
   

    if (failedTests > 0) {
        printf("\n****************OH NO*********!!!!! %d test(s) failed!\n", failedTests);
    } else {
        printf("\nAwesome! All tests passed!\n");
    }
   
    return 0;
}