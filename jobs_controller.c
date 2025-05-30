#include "jobs_controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

job_node sentinel = {.position = 0, .next = NULL};
job_list jobs = {.start = &sentinel, .end = &sentinel};

job_status* finished_jobs = NULL;

void add_job(const int pid, int status, const char* command) {
    // create job
    job_node *job = malloc(sizeof(job_node));
    job->position = jobs.end->position + 1;
    job->pid = pid;
    strcpy(job->status, status? "Running" : "Stopped");
    strcpy(job->command, command);
    job->next = NULL;

    // add job
    jobs.end->next = job;
    jobs.end = job;

    if (jobs.start == jobs.end) {
        jobs.start->next = job;  // First real job after sentinel
    }

    if (status == RUNNING) {
        printf("[%d] %6d\n", job->position, pid);
    } else {
        printf("[%d]   %-7s %24s", job->position, job->status, job->command);
    }
}

void delete_job(int pid) {
    job_node* job = jobs.start;
    while (job->next != NULL) {
        if (job->next->pid == pid) {
            job_node* temp = job->next;
            job->next = job->next->next;

            // Update jobs.end if we're removing the last job
            if (jobs.end == temp) {
                jobs.end = job;
            }

            free(temp);
            return;
        }
        job = job->next;
    }
}

void print_jobs() {
    for (job_node *job = jobs.start->next; job != NULL; job = job->next) {
        printf("[%d]   %-7s %24s", job->position, job->status, job->command);
    }
}

job_node* find_job(const int job_id) {
    job_node *job = jobs.start->next;
    while (job != NULL) {
        if (job->position == job_id) {
            return job;
        }
        job = job->next;
    }

    // Job not found
    return NULL;
}

void add_finished_job(const int pid) {
    // find job from pid
    job_node *finished_job = jobs.start->next;
    while (finished_job != NULL) {
        if (finished_job->pid == pid) {
            break;
        }
        finished_job = finished_job->next;
    }

    if (finished_job == NULL) {
        return;
    }

    job_status* new_status = malloc(sizeof(job_status));
    new_status->job_id = finished_job->position;
    strcpy(new_status->command, finished_job->command);
    new_status->next = NULL;

    if (finished_jobs == NULL) {
        finished_jobs = new_status;
    } else {
        job_status* temp = finished_jobs;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_status;
    }
}

void print_finished_jobs() {
    job_status* temp = finished_jobs;
    while (temp != NULL) {
        printf("[%d]   Done    %24s", temp->job_id, temp->command);
        job_status* to_free = temp;
        temp = temp->next;
        free(to_free);  // Clean up the queue after printing
    }
    finished_jobs = NULL;
}