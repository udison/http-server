#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

void throw_error(char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) throw_error("Failed to initialize socket");

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(8080)
    };

    if (connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw_error("Socket connection failed");
    }

    char* msg = "Hello Pelé, King of Football!";
    if (send(socket_fd, msg, strlen(msg), 0) < 0) {
        throw_error("Failed to send message to server");
    }

    char buf[1024] = {0};
    if (read(socket_fd, buf, 1024 - 1) < 0) {
        throw_error("Failed reading server response");
    }
    printf("%s\n", buf);

    close(socket_fd);
    
    return 0;
}
