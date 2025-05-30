#include "builtins.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "jobs_controller.h"
#include "signal_handler.h"

extern int exit_code;
extern char prev_command[];

void echo(char* str) {
    if (str == NULL) { return; }

    if (strcmp(str, "$?\n") == 0) {
        printf("%d\n", exit_code);
    }
    else {
        printf("%s", str);
    }
}

void double_bang(char* input, char* db_location) {
    char replaced_input[255] = {'\0'};

    *strchr(prev_command, '\n') = '\0'; // replace \n at the end with \0 so strcat works smoothly

    // input: 'smth!!smth\n\0'
    *db_location = '\0';    // input: 'smth\0!smth\n\0'
    strcat(replaced_input, input);  // replaced_input: 'smth\0'
    strcat(replaced_input, prev_command);   // replaced_input: 'smthprev_command\0'
    strcat(replaced_input, db_location+2);
    // replaced_input: 'smthprev_commandsmth\n\0'

    printf("%s", replaced_input);

    // replace input with replaced_input
    strcpy(input, replaced_input);
}

int my_exit(char* str) {
    if (str != NULL) {
        char* non_number = "";
        int code = (int) strtol(str, &non_number, 10) & 0xFF;
        if (non_number[0] == '\n') {
            exit_code = code;
            return 0;
        }
    }

    printf("Numeric argument required\n");
    return 2; // Incorrect usage, such as invalid options or missing arguments
}

void my_jobs() {
    print_jobs();
}

void fg(char *arg) {
    if (arg == NULL || arg[0] != '%' || !isdigit(arg[1])) {
        // handle invalid argument
        printf("fg: invalid job ID\n");
        return;
    }

    int job_id = atoi(&arg[1]);  // Convert %<job_id> to integer

    // Find the job with the specified job_id
    job_node *job = find_job(job_id);
    if (job == NULL) {
        // Job not found
        printf("fg: no such job\n");
        return;
    }

    // Set the job's process group to the foreground
    tcsetpgrp(STDIN_FILENO, job->pid);

    // Check the job status (if stopped, resume it)
    if (strcmp(job->status, "Stopped") == 0) {
        // Send SIGCONT to resume the stopped job
        kill(-job->pid, SIGCONT);
        strcpy(job->status, "Running");
    }

    block_tin_tou();

    // Wait for the foreground process to terminate
    int status;
    waitpid(-job->pid, &status, WUNTRACED);

    // Reset the terminal process group after the job finishes
    tcsetpgrp(STDIN_FILENO, getpid());

    unblock_tin_tou();

    delete_job(job->pid);
}

void bg(char *arg) {
    if (arg == NULL || arg[0] != '%' || !isdigit(arg[1])) {
        printf("bg: invalid job ID\n");
        return;
    }

    int job_id = atoi(&arg[1]);
    job_node *job = find_job(job_id);
    if (job == NULL) {
        printf("bg: job not found\n");
        return;
    }

    if (strcmp(job->status, "Stopped") != 0) {
        printf("bg: job %d already in background\n", job_id);
        return;
    }

    // Resume the job in the background
    kill(-job->pid, SIGCONT);
    strcpy(job->status, "Running");
    printf("[%d]  %s", job->position, job->command);
}