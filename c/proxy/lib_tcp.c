//
// Created by void on 3/20/23.
//

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib_tcp.h"

void write_to_file(char* path, char* buf) {
    int fd = open(path, O_CREAT | O_WRONLY, 0666);

    printf("Result buf to file: %s\n", buf);

    write(fd, buf, strlen(buf));
    close(fd);
}

int create_tcp_socket_fd() {
    int result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return result;
}

int send_message(int socket_fd, char *buf) {
    size_t msg_len = strlen(buf);

    int send_result = (int) send(socket_fd, buf, msg_len, 0);

    if (send_result < 0) {
        printf("Failed to send a message to client\n");
        return -1;
    }
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


int log_func(int func_result, char* log_msg) {
    printf("%s: ", log_msg);

    if (func_result < 0) { // == -1
        printf("FAILED\n");
        return -1;
    } else {
        printf("OK\n");
        return func_result;
    }
}