#include "builtins.h"

#include <stdio.h>

void echo(char* str) {
    printf("%s", str+5);
}
