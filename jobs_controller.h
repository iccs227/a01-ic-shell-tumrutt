#ifndef JOBS_CONTROLLER_H
#define JOBS_CONTROLLER_H

#define STOPPED 0
#define RUNNING 1

typedef struct job_node{
    int position;
    int pid;
    char status[8];
    char command[255];
    struct job_node* next;
} job_node;

typedef struct {
    job_node* start;
    job_node* end;
} job_list;

typedef struct job_status {
    int job_id;
    char command[255];
    struct job_status* next;
} job_status;

void add_job(const int pid, int status, const char* command);

void delete_job(int pid);

void print_jobs();

job_node* find_job(const int job_id);

void add_finished_job(const int pid);

void print_finished_jobs();

#endif //JOBS_CONTROLLER_H
