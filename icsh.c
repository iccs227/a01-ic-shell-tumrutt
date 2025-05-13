/* ICCS227: Project 1: icsh
 * Name: Thammarat Lapprakobkit
 * StudentID: 6381246
 */

#include "command_processor.h"
#include "stdio.h"
#include "welcome_banner.h"

#define MAX_CMD_BUFFER 255
extern int exit_code;

int main() {
    // welcome();

    char buffer[MAX_CMD_BUFFER];
    int resume = 1;
    while (resume) {
        printf("icsh $ ");
        fgets(buffer, 255, stdin);
        resume = process_command(buffer);
    }
    return exit_code;
}
