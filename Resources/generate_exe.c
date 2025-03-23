

#include <stdio.h>
#include <stdlib.h>

#define file2 "file2"

int main(void) {
    char cmd[100];
    FILE* f=fopen(file2 ".c", "w");

    fputs("#include <stdio.h>\n", f);
    fputs("int main(void) {\n", f);
    fputs("    printf(\"Hello World!\\n\");\n", f);
    fputs("}\n", f);

    fclose(f);

    sprintf(cmd, "gcc %s.c -o%s.exe", file2, file2);

    if (system(cmd)==0) {
        system(file2);
    } else {
        puts("Couldn't compile");
    }
}