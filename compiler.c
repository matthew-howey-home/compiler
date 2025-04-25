#include <stdio.h>

int main() {
    FILE *file = fopen("compiled_assembly.s", "w");  // Open file for writing
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return 1;
    }

    // Initial code
    fprintf(file,
        ".section .text\n"
        ".global main\n\n"
        "main:\n"
    );

    // main code
    fprintf(file, "push $2\n");

    // Output top of stack to exit code
    fprintf(file,
        "pop %%rax     # Top of stack into rax, 32 lowest bit will be in exit code\n"
        "ret               # Return from main\n"
    );

    fclose(file);  // Close the file

    printf("Assembly code written to compiled_assembly.s\n\n");
    return 0;
}