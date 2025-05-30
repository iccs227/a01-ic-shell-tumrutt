#include "command_processor.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"
#include "external.h"
#include "io_redirect.h"
#include "jobs_controller.h"
#include "signal_handler.h"

extern int update_command;
extern int redirected;
extern int exit_code;

int check_redirection(const char* command) {
    // Check for I/O redirection symbols and set up appropriate redirection
    char *input_redirect = strchr(command, '<');
    char *output_redirect = strchr(command, '>');

    if (input_redirect &&
        redirect_input(input_redirect) == -1) {
        return -1;
    }

    if (output_redirect &&
        redirect_output(output_redirect) == -1) {
        return -1;
    }

    // Null-terminate the input at the earliest redirection symbol
    char *first_redirect = NULL;
    if (input_redirect && output_redirect) {
        first_redirect = (input_redirect < output_redirect) ? input_redirect : output_redirect;
    } else {
        first_redirect = input_redirect ? input_redirect : output_redirect;
    }

    if (first_redirect) {
        *first_redirect = '\0';
        return 1;
    }

    return 0;
}

int is_background_process(const char* command) {
    char* ampersand_pos = strstr(command, "&\n");
    if (ampersand_pos != NULL) {
        *ampersand_pos = '\0';
        return 1;
    }
    return 0;
}

int process_command(char* input) {
    // ignore empty command
    if (strcmp(input, "\n") == 0 ||
        strcmp(input, "") == 0) {
        update_command = 0; // dont set this as prev_command
        return 1;
    }

    // replace !! with prev_command
    char* db_location = strstr(input, "!!");
    if (db_location != NULL) {
        double_bang(input, db_location);
    }

    // copy the input to retain the original for prev_command
    char input_copy[255];
    strcpy(input_copy, input);

    // check for redirection, terminate if -1
    redirected = check_redirection(input_copy);
    if (redirected == -1) {
        redirected = 0;
        return 1;
    }

    // check for &
    int background = is_background_process(input_copy);

    const char* command = strtok(input_copy, " \n");
    char* args = strtok(NULL, "");

    // check if exit
    if (strcmp(command, "exit") == 0) {
        return my_exit(args);
    }
    // fg and bg needs to be run outside of child process
    if (strcmp(command, "fg") == 0) {
        fg(args);
        return 1;
    }
    if (strcmp(command, "bg") == 0) {
        bg(args);
        return 1;
    }
    // not exit, fork and execute the command
    int status;
    const int child_pid = fork();
    if (child_pid == 0) {
        // set it as its own process group
        setpgrp();

        // call appropriate command
        if (strcmp(command, "echo") == 0) {
            echo(args);
            exit(0);
        }
        if (strcmp(command, "jobs") == 0) {
            my_jobs();
            exit(0);
        }
        int external_exit_code = exec_external(command, args);
        if (external_exit_code != -1) {
            exit(external_exit_code);
        }

        printf("bad command\n");
        exit(127);
    } else {
        // parent
        setpgid(child_pid, child_pid);
        // if "&\n" is found, dont wait.
        if (background) {
            add_job(child_pid, RUNNING, input);
            return 1;
        }
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
        // if ^Z, add stopped process to the job list
        if (WIFSTOPPED(status)) {
            add_job(child_pid, STOPPED, input);
        }
    }
    else {
        exit_code = WEXITSTATUS(status);
    }

    return 1;
}
