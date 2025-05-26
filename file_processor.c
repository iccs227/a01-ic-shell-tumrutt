#include "file_processor.h"

#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "command_processor.h"

#define MAX_CMD_BUFFER 255
extern int exit_code;

void process_file(const char *file_location) {
    const char *ext =  strrchr(file_location, '.');
    if (ext == NULL || strcmp(ext, ".sh") != 0) {
        printf("Unsupported file type.\n");
        exit_code = 65; // input data format is incorrect or unexpected.
        return;
    }

    FILE* file = fopen(file_location, "r");

    if (file == NULL) {
        printf("Failed to open %s.\n", file_location);
        perror("");
        exit_code = 67; // invalid or unknown destination address.
        return;
    }

    int resume = 1;
    while (resume) {
        char buffer[MAX_CMD_BUFFER];
        if (fgets(buffer, MAX_CMD_BUFFER, file) == NULL) {
            printf("\nExit command not found. Terminating the program with exit code 0\n");
            exit_code = 0;
            break;
        }
        resume = process_command(buffer);
    }

    fclose(file);
}
