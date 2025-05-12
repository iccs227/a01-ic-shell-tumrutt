#include "command_processor.h"

#include <stdio.h>

#include "builtins.h"
#include "string.h"

void process_command(char* input) {
    char* command = strtok(input, " ");

    if (strcmp(input, "\n") == 0) {
        // skipppp
    }
    else if (strcmp(command, "echo") == 0) {
        echo(input);
    }
    else {
        printf("bad command\n");
    }
}