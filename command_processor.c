#include "command_processor.h"

#include <stdio.h>

#include "builtins.h"
#include "external.h"
#include "io_redirect.h"
#include "string.h"

extern int update_command;
extern int redirected;

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

    const char* command = strtok(input_copy, " \n");
    char* args = strtok(NULL, "");

    // call appropriate command
    if (strcmp(command, "echo") == 0) {
        echo(args);
        return 1;
    }
    if (strcmp(command, "exit") == 0) {
        return my_exit(args);
    }
    if (exec_external(command, args) != 255) {
        return 1;
    }

    printf("bad command\n");
    return 1;
}
