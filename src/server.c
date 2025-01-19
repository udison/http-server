#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

void throw_error(char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        throw_error("Failed to create socket fd");
    }

    int opt = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        throw_error("Failed to set socket address/port reusability");
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(8080)
    };
    int addr_len = sizeof(addr);
    if (bind(socket_fd, (struct sockaddr*)&addr, addr_len) < 0) {
        throw_error("Failed to bind socket");
    }

    if (listen(socket_fd, 3) < 0) {
        throw_error("Failed to listen for connections");
    }
    printf("Server listening on port 8080...\n");

    static struct sockaddr_in cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);

    for (int hit = 1;; hit++) {
        int conn_fd = accept(socket_fd, (struct sockaddr*)&cli_addr, &cli_addr_len); 
        if (conn_fd < 0) {
            throw_error("Error accepting connection");
        }

        char buffer[1024] = {0};
        ssize_t val_read = read(conn_fd, buffer, 1024 - 1);
        printf("============ Connection %i ============ \n", hit);
        printf("%s\n\n", buffer);

        char* response = "Hello Mark! King of Zapi-Zapi.";
        char header[1024] = {0};
        sprintf(header,
            "HTTP/1.1 200 OK\n"
            "Server: rock/1.0\n"
            "Content-Type: text/plain\n"
            "Content-Length: %ld\r\n",
            0l //strlen(response)
        );

        printf("Sending Header: %s\n", header);
        if (write(conn_fd, header, strlen(header)) < 0) {
            throw_error("Failed to send header");
        }

        // printf("Sending Response: %s\n", response);
        // if (write(conn_fd, response, strlen(response)) < 0) {
        //     throw_error("Failed to send response");
        // }
        printf("======== End of connection %i ========= \n", hit);

        close(conn_fd);
    }

    close(socket_fd);
    return 0;
}
