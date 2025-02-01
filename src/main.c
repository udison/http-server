#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include "logger.h"
#include "server.h"

int main(int argc, char* argv[]) {
    // for (int i = 1; i < argc; i++) {
    //     printf("%i: %s\n", i, argv[i]);
    // }

    if (argc < 2 || argv[1][0] == '-') {
        errno = EINVAL;
        throw_error("Port must be a positive integer");
    }

    Server server;
    server_init(&server);

    uint16_t port = atoi(argv[1]);
    server_listen(&server, port);

    server_close(&server);

    return 0;
}
