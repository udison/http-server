#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
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

    static struct sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);

    for (int hit = 1;; hit++) {
        int conn_fd = accept(server.sock_fd, (struct sockaddr*)&cli_addr, &cli_addr_len); 
        if (conn_fd < 0) {
            throw_error("Error accepting connection");
        }

        char buffer[1024] = {0};
        ssize_t val_read = read(conn_fd, buffer, 1024 - 1);
        printf("============ Connection %i ============ \n", hit);
        printf("> Request\n%s\n\n", buffer);

        char* msg = "Hello Mark! King of Zapi-Zapi.";
        char response[1024] = {0};
        sprintf(response,
            "HTTP/1.1 200 OK\n"
            "Server: rock/1.0\n"
            "Content-Type: text/plain\n"
            "Content-Length: %ld\n"
                "\n"
                "%s\r\n",
            strlen(msg),
            msg
        );

        printf("> Response\n%s\n", response);
        if (write(conn_fd, response, strlen(response)) < 0) {
            throw_error("Failed to send response");
        }
        printf("======== End of connection %i ========= \n", hit);

        close(conn_fd);
    }

    return 0;
}
