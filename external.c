#include "external.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int exec_external(const char* input) {
    char input_copy[255];
    char *args[100];

    strcpy(input_copy, input);
    char* command = strtok(input_copy, " \n");

    int i = 0;
    while (command != NULL) {
        args[i++] = command;
        command = strtok(NULL, " \n");
    }
    args[i] = NULL;

    int status;
    if (fork() == 0){
        // child -> run external command
        execvp(args[0], args);
        exit(1); // return 1 when process fail
    }
    else {
        // parent
        wait(&status);
    }

    return status;
}
