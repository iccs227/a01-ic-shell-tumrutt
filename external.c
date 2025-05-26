#include "external.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "signal_handler.h"

extern int exit_code;

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
    const int child_pid = fork();
    if (child_pid == 0) {
        // child -> run external command
        setpgrp(); // make a new process group for it
        execvp(args[0], args);
        exit(255); // return 255 when process fail
    }
    else {
        // parent
        // set the child process as foreground
        tcsetpgrp(STDIN_FILENO, child_pid);

        // temporarily block SIGTTIN and SIGTTOU so the shell doesn't get suspended
        block_tin_tou();

        // wait for the child to terminate or suspended and restore shell as foreground
        waitpid(child_pid, &status, WUNTRACED);
        tcsetpgrp(STDIN_FILENO, getpid());

        // unblock SIGTTIN and SIGTTOU
        unblock_tin_tou();
    }

    // if ^C or ^Z,  print new line so it looks nice
    if (WIFSIGNALED(status) || WIFSTOPPED(status)) {
        printf("\n");
        const int signal = WIFSIGNALED(status) ? WTERMSIG(status) : WSTOPSIG(status);
        exit_code = 128 + signal;
    }
    else {
        exit_code = WEXITSTATUS(status);
    }

    return WEXITSTATUS(status);
}
