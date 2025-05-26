#include "signal_handler.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int signum) {
    printf("\nicsh $ ");
    fflush(stdout);
}

void setup_signal_handler() {
    struct sigaction my_handler;
    sigemptyset(&my_handler.sa_mask);
    my_handler.sa_handler = handler;
    my_handler.sa_flags = SA_RESTART;
    sigaction(SIGINT, &my_handler, NULL);
    sigaction(SIGTSTP, &my_handler, NULL);
}

// Block signals temporarily (SIGTTIN and SIGTTOU can suspend the shell)
void block_tin_tou() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGTTIN);  // Block SIGTTIN (background read)
    sigaddset(&set, SIGTTOU);  // Block SIGTTOU (background write)
    sigprocmask(SIG_BLOCK, &set, NULL);
}

// Unblock the signals after the task is done
void unblock_tin_tou() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGTTIN);
    sigaddset(&set, SIGTTOU);
    sigprocmask(SIG_UNBLOCK, &set, NULL);
}