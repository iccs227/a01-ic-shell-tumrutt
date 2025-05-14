#include "builtins.h"

#include <stdio.h>
#include <stdlib.h>

extern int exit_code;
extern int update_command;

void echo(char* str) {
    printf("%s", str+5);
}

void double_bang() {
    update_command = 0;
}

void my_exit(char* str) {
    exit_code = strtol(str+5, NULL, 10) & 0xFF;
}
