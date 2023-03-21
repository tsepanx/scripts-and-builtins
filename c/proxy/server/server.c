//
// Created by void on 3/21/23.
//


#include "../lib_tcp.h"
#include "../utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MESSAGE_SIZE 2000
#define MAX_CLIENTS 10

int SERVER_PORT;
struct sockaddr_in server_addr;
char client_message[MESSAGE_SIZE], server_message[MESSAGE_SIZE];

char *svg_begin = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\" style=\"shape-rendering:geometricPrecision;text-rendering:geometricPrecision;image-rendering:optimizeQuality;fill-rule:evenodd;clip-rule:evenodd\"><path fill=\"#7fbc00\" d=\"M803 335c7-1 13 2 18 9l92 164c7 10 6 19-2 28L795 639l-2 103c-2 7-7 11-14 12H213c-9-2-14-7-16-15l-1-37-21-29-3-11 23-191c3-10 10-15 20-15l179 13 409-134Zm-11 43c3 0 5 1 7 4l75 134-9 10a7347 7347 0 0 0-108 99l-1 86-2 4-259 2-257-1-2-1-3-26a468 468 0 0 0-22-36l18-149c0-6 2-9 7-9l165 11 391-128Z\" style=\"opacity:.974\"/><path fill=\"#7fbc00\" d=\"M675 453c11-3 19 2 23 13v201c-3 7-9 12-18 13-9-1-15-6-18-13V465c3-6 7-10 13-12Z\" style=\"opacity:.994\"/><path fill=\"#7fbc00\" d=\"M770 453c11-3 19 2 23 13v77c-7 12-16 16-29 9-4-2-6-6-7-9v-78c3-6 7-10 13-12Z\" style=\"opacity:.981\"/><path fill=\"#7fbc00\" d=\"M487 505c10-1 17 3 21 13v162c-3 8-9 12-18 13-9-1-15-5-18-13V518c3-8 7-12 15-13Z\" style=\"opacity:.991\"/><path fill=\"#7fbc00\" d=\"M581 516c10-2 18 3 22 13v108c-3 7-9 12-18 13-9-1-15-6-18-13V529c2-7 7-11 14-13Z\" style=\"opacity:.988\"/><path fill=\"#7fbc00\" d=\"M299 528c11-1 18 4 21 16v129c-3 12-11 17-24 15-7-2-11-7-13-13V542c3-8 8-13 16-14Z\" style=\"opacity:.963\"/><path fill=\"#7fbc00\" d=\"M390 562c12-3 20 2 24 14v84c-4 11-12 16-24 13-8-3-12-8-13-15v-80c1-8 5-14 13-16Z\" style=\"opacity:.968\"/>";
char *svg_text = "<text x=\"350\" y=\"840\" style=\"font-size:72px;fill:#000\">Hello, %s!</text>";
char *svg_end = "</svg>";

int server_main(int conn_socket_fd) {
    // Receive the client's username
    if (recv(conn_socket_fd, client_message, sizeof(client_message), 0) < 0)
    {
        printf("Failed to receive a message from client\n");
        exit(EXIT_FAILURE);
    } else { printf("Receive: OK\n"); }

    printf("Client's message: %s\n", client_message);

    // Send a vector image to the client
    send_message(conn_socket_fd, svg_begin);
    sprintf(server_message, svg_text, client_message);
    send_message(conn_socket_fd, server_message);
    send_message(conn_socket_fd, svg_end);

    return 0;
}

int main(int argn, char** argv) {
    SERVER_PORT = get_server_port(argn, argv);

    // === CREATE SOCKET ===
    wait_interrupt("Create socket?");

    // Create 'Accepting' TCP socket
    int accepting_socket_fd = create_tcp_socket_fd();
    if (log_func(accepting_socket_fd, "Create ACCEPTING socket") < 0) exit(EXIT_FAILURE);

    // Set IP and Port
    setup_server_addr(&server_addr, SERVER_PORT);

    // === BIND SOCKET TO HOST:PORT
    wait_interrupt("Bind socket?");

    int bind_code = bind_socket(accepting_socket_fd, &server_addr);
    if (log_func(bind_code, "BIND socket") < 0) exit(EXIT_FAILURE);

    // === LISTEN ===
    wait_interrupt("Listen?");

    int listen_code = listen(accepting_socket_fd, MAX_CLIENTS);
    if (log_func(listen_code, "Listening") < 0) exit(EXIT_FAILURE);

    printf("=== Server is listening on \"%s:%d\" ===\n", SERVER_IP_ADDR, SERVER_PORT);

    // === ACCEPT (CREATE CONN_SOCKET) ===

    while (1) {
        wait_interrupt("Accept (conn_socket create)?");

        // Create new socket for receive/sending packets
        struct sockaddr client_addr;
        int conn_socket_fd = accept_conn(accepting_socket_fd, &client_addr);
        if (log_func(listen_code, "Conn socket_df") < 0) exit(EXIT_FAILURE);


        // === SERVER MAIN ===
        wait_interrupt("Server main?");

        int main_code = server_main(conn_socket_fd);
        log_func(main_code, "Server main"); //  < 0)

        printf("Closing conn socket: (%d)", conn_socket_fd);
        close(conn_socket_fd);
    }
}