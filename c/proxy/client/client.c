//
// Created by void on 3/21/23.
//

#include "../lib_tcp.h"
#include "../utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int SERVER_PORT;
struct sockaddr_in server_addr;

int client_main(int socket_fd) {
    char* input_name = malloc(sizeof(char) * 100);

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

int main(int argn, char** argv) {
    SERVER_PORT = get_server_port(argn, argv);

    // Set IP and Port
    setup_server_addr(&server_addr, SERVER_PORT);

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