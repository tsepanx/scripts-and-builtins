//
// Created by void on 3/20/23.
//

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "lib_tcp.h"
#include "utils.h"

int accept_conn(int accepting_socket_fd, struct sockaddr* client_addr) {
    int addr_len = sizeof(&client_addr);

    int conn_socket_fd = accept(accepting_socket_fd, client_addr, (socklen_t*) &addr_len);
    return conn_socket_fd;
}

int bind_socket(int accepting_socket_fd, struct sockaddr_in* server_addr) {
    int server_addr_size = sizeof(*server_addr);

    int bind_code = bind(accepting_socket_fd, (struct sockaddr*) server_addr, server_addr_size);
    return bind_code;
}

int create_tcp_socket_fd() {
    int result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return result;
}

int send_message(int socket_fd, char *buf) {
    size_t msg_len = strlen(buf);

    int send_result = (int) send(socket_fd, buf, msg_len, 0);
    return send_result;
}

int receive_message(int socket_fd, char* buf, int size) {
    ssize_t recv_result = recv(socket_fd, buf, size, 0);

    if (recv_result < 0) {
        printf("Failed to receive a message from client\n");
        return -1;
    }

    return (int) recv_result;
}

int connect_to_addr(int socket_fd, struct sockaddr_in server_addr) {
    int server_addr_size = sizeof(server_addr);
    int conn_result = connect(socket_fd, (const struct sockaddr *) &server_addr, server_addr_size);

    if (conn_result != 0) {
        return -1;
    } else {
        return 0;
    }
}


