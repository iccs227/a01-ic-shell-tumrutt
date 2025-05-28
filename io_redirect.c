#include "io_redirect.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

int shell_input = -1;
int shell_output = -1;

char* find_file_name(char* redirector_location) {
    // find the start of file name
    char* start_of_file_name = redirector_location + 1;
    while (isspace(*start_of_file_name)) {
        start_of_file_name += 1;
    }

    // find the end of file name
    char* end_of_file_name = start_of_file_name;
    while (isalpha(*end_of_file_name) || *end_of_file_name == '.') {
        end_of_file_name += 1;
    }

    // copy the file name to file_name
    char* file_name = malloc(end_of_file_name - start_of_file_name + 1);
    strncpy(file_name, start_of_file_name, end_of_file_name - start_of_file_name);

    return file_name;
}

int redirect_input(char* redirector_location) {
    char* file_name = find_file_name(redirector_location);

    // redirect input to file
    int file = open(file_name, O_RDONLY);
    free(file_name);
    if (file == -1) {
        printf("No such file or directory\n");
        return -1;
    }
    shell_input = dup(STDIN_FILENO);
    dup2(file, STDIN_FILENO);
    close(file);

    return 0;
}

int redirect_output(char* redirector_location) {
    char* file_name = find_file_name(redirector_location);

    // redirect output to file
    int file = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    free(file_name);
    if (file == -1) {
        printf("Error opening/creating file\n");
        return -1;
    }
    shell_output = dup(STDOUT_FILENO);
    dup2(file, STDOUT_FILENO);
    close(file);

    return 0;
}

void restore_io() {
    if (shell_input != -1) {
        dup2(shell_input, STDIN_FILENO);
        close(shell_input);
    }
    if (shell_output != -1) {
        dup2(shell_output, STDOUT_FILENO);
        close(shell_output);
    }
}