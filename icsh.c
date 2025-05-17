/* ICCS227: Project 1: icsh
 * Name: Thammarat Lapprakobkit
 * StudentID: 6381246
 */

#include <unistd.h>
#include <string.h>

#include "command_processor.h"
#include "stdio.h"
#include "welcome_banner.h"

#define MAX_CMD_BUFFER 255
int exit_code = 0;
int update_command = 1; // boolean determining whether to update command or not
char buffer[MAX_CMD_BUFFER];
char prev_command[MAX_CMD_BUFFER];

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
        if (fgets(buffer, MAX_CMD_BUFFER, file) == NULL) {
            printf("\nExit command not found. Terminating the program with exit code 0\n");
            exit_code = 0;
            break;
        }
        resume = process_command(buffer);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc > 1) { // process the file(s) and exit
        for (int i = 1; i < argc; ++i) {
            process_file(argv[i]);
        }
        return exit_code;
    }

    // welcome();

    int resume = 1;

    while (resume) {
        printf("icsh $ ");
        fgets(buffer, 255, stdin);

        resume = process_command(buffer);

        if (update_command) {
            strcpy(prev_command, buffer);
        }
        else {
            update_command = 1;
        }
    }
    return exit_code;
}
