#include "logger.h"
#include <stdio.h>

void throw_error(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void log_error(const char* msg) {
    printf("Error: %s\n", msg);
}
