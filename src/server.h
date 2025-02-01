#pragma once

#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>

typedef struct Server { 
    struct sockaddr_in addr;
    int sock_fd;
} Server;

void server_init(Server* sv);
void server_listen(Server* sv, uint16_t port);
void server_close(Server* sv);

// Intended way to register new routes
// void server_get(*sock_fd, "/", callback);
// void server_post(*sock_fd, "/users", usersCallback);
