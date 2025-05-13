#include "builtins.h"

#include <stdio.h>
#include <stdlib.h>

int exit_code = 0;

void echo(char* str) {
    printf("%s", str+5);
}

void my_exit(char* str) {
    exit_code = strtol(str+5, NULL, 10) & 0xFF;
}
