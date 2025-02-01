#include "server.h"
#include "logger.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>


void server_init(Server* server) {
    // Initializes the socket
    server->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sock_fd < 0) {
        throw_error("Failed to create socket fd");
    }

    // Reuses the port and address if server goes poo poo
    int opt = 1;
    if (setsockopt(server->sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        throw_error("Failed to set socket address/port reusability");
    }
}

void server_close(Server* sv) {
    close(sv->sock_fd);
}

void server_listen(Server* sv, uint16_t port) {
    if (port <= 0) {
        errno = EINVAL;
        throw_error("Port must be a positive integer");
    }

    sv->addr.sin_family = AF_INET;
    sv->addr.sin_addr.s_addr = INADDR_ANY;
    sv->addr.sin_port = htons(port);

    int addr_len = sizeof(sv->addr);

    if (bind(sv->sock_fd, (struct sockaddr*)&sv->addr, addr_len) < 0) {
        throw_error("Failed to bind socket");
    }

    if (listen(sv->sock_fd, 3) < 0) {
        throw_error("Failed to listen for connections");
    }

    printf("Server listening on port %i...\n", port);

    static struct sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);

    for (int hit = 1;; hit++) {
        int conn_fd = accept(sv->sock_fd, (struct sockaddr*)&cli_addr, &cli_addr_len); 
        if (conn_fd < 0) {
            log_error("Error accepting connection");
        }

        char buffer[1024] = {0};
        ssize_t val_read = read(conn_fd, buffer, 1024 - 1);
        printf("============ Connection %i ============ \n", hit);
        printf("> Request\n%s\n\n", buffer);

        char* msg = "Hello Mark! King of Zapi-Zapi.";
        char response[1024] = {0};
        sprintf(response,
            "HTTP/1.1 200 OK\n"
            "Server: %s/%s\n"
            "Content-Type: text/plain\n"
            "Content-Length: %ld\n"
                "\n"
                "%s\r\n",
            APP_NAME, APP_VERSION,
            strlen(msg),
            msg
        );

        printf("> Response\n%s\n", response);
        if (write(conn_fd, response, strlen(response)) < 0) {
            log_error("Failed to send response");
        }
        printf("======== End of connection %i ========= \n", hit);

        close(conn_fd);
    }
}

