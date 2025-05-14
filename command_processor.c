#include "command_processor.h"

#include <stdio.h>

#include "builtins.h"
#include "string.h"

extern char prev_command[];

int process_command(char* input) {
    char input_copy[255];
    strcpy(input_copy, input);
    char* command = strtok(input_copy, " ");

    if (strcmp(input, "\n") == 0 ||
        strcmp(input, "") == 0) {
        return 1;
    }
    if (strcmp(command, "echo") == 0) {
        echo(input);
        return 1;
    }
    if (strcmp(command, "!!\n") == 0) {
        double_bang();
        printf("%s", prev_command);
        return process_command(prev_command);
    }
    if (strcmp(command, "exit") == 0) {
        my_exit(input);
        return 0;
    }

    printf("bad command\n");
    return 1;
}
