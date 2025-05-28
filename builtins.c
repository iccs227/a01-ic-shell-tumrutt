#include "builtins.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int exit_code;
extern char prev_command[];

void echo(char* str) {
    if (str == NULL) { return; }

    if (strcmp(str, "$?\n") == 0) {
        printf("%d\n", exit_code);
    }
    else {
        printf("%s", str);
        exit_code = 0;
    }
}

void double_bang(char* input, char* db_location) {
    char replaced_input[255] = {'\0'};

    *strchr(prev_command, '\n') = '\0'; // replace \n at the end with \0 so strcat works smoothly

    // input: 'smth!!smth\n\0'
    *db_location = '\0';    // input: 'smth\0!smth\n\0'
    strcat(replaced_input, input);  // replaced_input: 'smth\0'
    strcat(replaced_input, prev_command);   // replaced_input: 'smthprev_command\0'
    strcat(replaced_input, db_location+2);
    // replaced_input: 'smthprev_commandsmth\n\0'

    printf("%s", replaced_input);

    // replace input with replaced_input
    strcpy(input, replaced_input);

    exit_code = 0;
}

int my_exit(char* str) {
    char* non_number = "";

    if (str != NULL) {
        exit_code = strtol(str, &non_number, 10) & 0xFF;
    }

    if (non_number[0] != '\n') {
        printf("Numeric argument required\n");
        exit_code = 2; // Incorrect usage, such as invalid options or missing arguments
        return 1;
    }
    return 0;
}
