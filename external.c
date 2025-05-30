#include "external.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "signal_handler.h"

extern int exit_code;

int exec_external(const char* command, char* args) {
    char *argv[100];

    argv[0] = (char*)command;

    int i = 1;
    char* arg = strtok(args, " \n");
    while (arg != NULL) {
        argv[i++] = arg;
        arg = strtok(NULL, " \n");
    }
    argv[i] = NULL;

    execvp(argv[0], argv);

    return -1; // return -1 if execvp fail
}
