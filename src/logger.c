#include "logger.h"

void throw_error(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
