#include "builtins.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int exit_code;
extern int update_command;

void echo(char* str) {
    if (strcmp(str+5, "$?\n") == 0) {
        printf("%d\n", exit_code);
    }
    else {
        printf("%s", str+5);
        exit_code = 0;
    }
}

void double_bang() {
    update_command = 0;
    exit_code = 0;
}

int my_exit(char* str) {
    char* non_number;
    exit_code = strtol(str+5, &non_number, 10) & 0xFF;

    if (non_number[0] != '\n') {
        printf("Numeric argument required\n");
        exit_code = 2; // Incorrect usage, such as invalid options or missing arguments
        return 1;
    }
    return 0;
}
