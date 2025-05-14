/* ICCS227: Project 1: icsh
 * Name: Thammarat Lapprakobkit
 * StudentID: 6381246
 */

#include <string.h>

#include "command_processor.h"
#include "stdio.h"
#include "welcome_banner.h"

#define MAX_CMD_BUFFER 255
int exit_code = 0;
int update_command = 1; // boolean determining whether to update command or not
char prev_command[MAX_CMD_BUFFER];

int main() {
    // welcome();

    char buffer[MAX_CMD_BUFFER];
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
