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

    int status;
    const int child_pid = fork();
    if (child_pid == 0) {
        // child -> run external command
        setpgrp(); // make a new process group for it
        execvp(argv[0], argv);
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
