/* ICCS227: Project 1: icsh
 * Name: Thammarat Lapprakobkit
 * StudentID: 6381246
 */

#include "command_processor.h"
#include "stdio.h"
#include "welcome_banner.h"

#define MAX_CMD_BUFFER 255

int main() {
    // welcome();

    char buffer[MAX_CMD_BUFFER];
    while (1) {
        printf("icsh $ ");
        fgets(buffer, 255, stdin);
        process_command(buffer);
    }
}
