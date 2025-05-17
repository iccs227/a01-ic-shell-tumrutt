#include "command_processor.h"

#include <stdio.h>

#include "builtins.h"
#include "external.h"
#include "string.h"

extern char prev_command[];

int process_command(char* input) {
    if (strcmp(input, "\n") == 0 ||
        strcmp(input, "") == 0) {
        return 1;
    }
    if (strncmp(input, "echo", 4) == 0) {
        echo(input);
        return 1;
    }
    if (strcmp(input, "!!\n") == 0) {
        double_bang();
        printf("%s", prev_command);
        return process_command(prev_command);
    }
    if (strncmp(input, "exit", 4) == 0) {
        my_exit(input);
        return 0;
    }
    if (exec_external(input) == 0) {
        return 1;
    }

    printf("bad command\n");
    return 1;
}
