//
// Created by void on 3/21/23.
//

#include "lib_tcp.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int client_main(int socket_fd) {
    char* input_name = malloc(sizeof(char) * 100);;

    printf("Enter name: ");
    scanf("%s", input_name);

    int send_res = send_message(socket_fd, input_name);
    if (log_func(send_res, "Sending") == -1) return -1;

    int msg_size = 2000;
    char server_msg[msg_size];

    int recv_res = receive_message(socket_fd, server_msg, msg_size);
    if (log_func(recv_res, "Receiving") == -1) return -1;

    write_to_file("img.svg", server_msg);

    return 0;
}

void wait_interrupt(char* msg_print) {
    printf("%s", msg_print);
    char *q1 = malloc(sizeof(char) * 10);

    scanf("%s", q1);
}

#define SERVER_IP_ADDR "127.0.0.1"
#define DEFAULT_SERVER_PORT 2022
int SERVER_PORT; // f.e. 2022

int main(int argn, char** argv) {
    if (argn > 1) {
        SERVER_PORT = (int) strtol(argv[1], NULL, 10);
    } else {
//        printf("You haven't provided SERVER PORT");
//        return -1;
        SERVER_PORT = DEFAULT_SERVER_PORT;
        printf("setting SERVER_PORT to default: %d", SERVER_PORT);
    }

    struct sockaddr_in server_addr;
    // Set IP and Port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    server_addr.sin_port = htons(SERVER_PORT);

    // === CREATE SOCKET ===

    wait_interrupt("Create socket?");

    int socket_fd = create_tcp_socket_fd();
    if (log_func(socket_fd, "Create socket") == -1) return -1;

    // === CONNECT TO SOCKET ===

    wait_interrupt("Connect to socket?");
    printf("Connecting: %s : %d\n", SERVER_IP_ADDR, SERVER_PORT);

    int conn_code = connect_to_addr(socket_fd, server_addr);
    if (log_func(conn_code, "Connect") == -1) return -1;

    // === CLIENT MAIN ===

    wait_interrupt("Client main?");
    int main_code = client_main(socket_fd);
    if (log_func(main_code, "Client main") == -1) return -1;

    // === CLOSE SOCKET ===

    wait_interrupt("Close sockets?");
    printf("Closing socket...\n");
    close(socket_fd);

    return 0;
}