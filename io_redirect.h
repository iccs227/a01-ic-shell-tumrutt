#ifndef IO_REDIRECT_H
#define IO_REDIRECT_H

int redirect_input(char* redirector_location);

int redirect_output(char* redirector_location);

void restore_io();

#endif //IO_REDIRECT_H
