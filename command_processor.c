#include "command_processor.h"

#include <stdio.h>

#include "builtins.h"
#include "string.h"

int process_command(char* input) {
    char* command = strtok(input, " ");

    if (strcmp(input, "\n") == 0) {
        return 1;
    }
    if (strcmp(command, "echo") == 0) {
        echo(input);
        return 1;
    }
    if (strcmp(command, "exit") == 0) {
        my_exit(input);
        return 0;
    }

    printf("bad command\n");
    return 1;
}
