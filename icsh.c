/* ICCS227: Project 1: icsh
 * Name: Thammarat Lapprakobkit
 * StudentID: 6381246
 */

#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "command_processor.h"
#include "file_processor.h"
#include "io_redirect.h"
#include "jobs_controller.h"
#include "signal_handler.h"
#include "stdio.h"
#include "welcome_banner.h"

#define MAX_CMD_BUFFER 255
int exit_code = 0;
int update_command = 1; // boolean determining whether to update command or not
char prev_command[MAX_CMD_BUFFER];
int redirected = 0;

int main(int argc, char *argv[]) {
    if (argc > 1) { // process the file(s) and exit
        for (int i = 1; i < argc; ++i) {
            process_file(argv[i]);
        }
        return exit_code;
    }

    welcome();

    setup_signal_handler();
    int resume = 1;

    while (resume) {
        char buffer[MAX_CMD_BUFFER];
        printf("icsh $ ");
        fgets(buffer, 255, stdin);

        resume = process_command(buffer);

        print_finished_jobs();

        if (redirected) {
            // return I/O to stdin and stdout
            restore_io();
            redirected = 0;
        }

        if (update_command) {
            strcpy(prev_command, buffer);
        }
        else {
            update_command = 1;
        }
    }
    return exit_code;
}
