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

    int conn_fd = accept(socket_fd, (struct sockaddr*)&addr, &addr_len);
    if (conn_fd < 0) {
        throw_error("Error accepting connection");
    }

    char buffer[1024] = {0};
    ssize_t val_read = read(conn_fd, buffer, 1024 - 1);
    printf("%s\n", buffer);

    char* response = "Hello Mark! King of Zapi-Zapi.";
    if (send(conn_fd, response, strlen(response), 0) < 0) {
        throw_error("Failed to send response");
    }

    close(conn_fd);
    close(socket_fd);

    return 0;
}
