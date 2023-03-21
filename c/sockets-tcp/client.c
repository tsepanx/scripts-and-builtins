#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <fcntl.h>


#define SERVER_IP_ADDR "127.0.0.1"
//#define SERVER_PORT 2001

#define SLEEP_USECONDS 1000000

void write_to_file(char* fpath, char* string) {
    int fd = open(fpath, O_CREAT | O_WRONLY, 0666);

    printf("Result string to file: %s\n", string);

    write(fd, string, strlen(string));
    close(fd);
}

int create_socket_fd() {
    usleep(SLEEP_USECONDS);
    int result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return result;
}

// Sends a message to the accepted client through the connection socket.
int send_message(char *msg, int socket_fd) {
    size_t msg_len = strlen(msg);

//    usleep(SLEEP_USECONDS);
    ssize_t send_result = send(socket_fd, msg, msg_len, 0);

    if (send_result < 0) {
        printf("Failed to send a message to client\n");
        return -1;
    }
    return (int) send_result;
}

int receive_message(int socket_fd, char* buffer, int size) {
//    usleep(SLEEP_USECONDS);
    ssize_t recv_result = recv(socket_fd, buffer,size,0);

    if (recv_result < 0) {
        printf("Failed to receive a message from client\n");
        return -1;
    }

    return (int) recv_result;
}

int connect_to_addr(int socket_fd, struct sockaddr_in server_addr) {
    int server_addr_size = sizeof(server_addr);

    usleep(SLEEP_USECONDS);
    int connect_result = connect(socket_fd, (const struct sockaddr *) &server_addr, server_addr_size);
    return connect_result;
}

int client_main(int socket_fd) {
    char* input_name = malloc(sizeof(char) * 100);;

    printf("Enter name: ");
    scanf("%s", input_name);
    send_message(input_name, socket_fd);

    int msg_size = 2000;
    char server_msg[msg_size];

    int recv_result = receive_message(socket_fd, server_msg, msg_size);
    if (recv_result < 0) {
        printf("Receive: FAILED\n");
        return -1;
    } else {
        printf("Receive: OK\n");
    }

    write_to_file("img.svg", server_msg);

    return 0;
}

void wait_interrupt(char* msg_print) {
    printf("%s", msg_print);
    char *q1 = malloc(sizeof(char) * 10);

    scanf("%s", q1);
}

int main(int argn, char** argv) {
    int SERVER_PORT;
    if (argn > 1) {
        SERVER_PORT = atoi(argv[1]);
    } else {
        printf("You haven't provided SERVER PORT");
        return -1;
    }

    struct sockaddr_in server_addr;
    // Set IP and Port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    server_addr.sin_port = htons(SERVER_PORT);

    // === CREATE SOCKET ===

    wait_interrupt("Create socket?");

    int socket_fd = create_socket_fd();
    if (socket_fd == -1) {
        printf("Create socket: FAILED\n");
        return -1;
    } else {
        printf("Create socket: OK\n");
    }

    // === CONNECT TO SOCKET ===

    wait_interrupt("Connect to socket?");
    printf("Connecting: %s : %d\n", SERVER_IP_ADDR, SERVER_PORT);

    int conn_code = connect_to_addr(socket_fd, server_addr);
    if (conn_code != 0) {
        printf("Connect: FAILED\n");
        return -1;
    } else {
        printf("Connect: OK (%d)\n", socket_fd);
    }

    // === CLIENT MAIN ===

    wait_interrupt("Client main?");

    int main_code = client_main(socket_fd);
    if (main_code != 0) {
        return -1;
    }

    // === CLOSE SOCKET ===

    wait_interrupt("Close sockets?");

    printf("Closing socket...\n");
    close(socket_fd);

    return 0;
}