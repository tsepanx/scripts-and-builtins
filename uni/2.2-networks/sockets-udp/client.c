#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP_ADDR "127.0.0.1"
#define SERVER_PORT 2000
#define SECRET_NUMBER 42

struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
int client_struct_length = sizeof(client_addr);

// Function to send a message to the client
int send_message(
    int socket_fd,
    struct sockaddr_in socket_address,
    char* buff
) {
    usleep(300000);
    int send_result = sendto(
        socket_fd,
        buff,
        strlen(buff),
        0,
        (struct sockaddr *)&socket_address,
        sizeof(socket_address)
    );

    if (send_result < 0) {
        printf("Failed to send a message to client\n");
        return -1;
    }

    return 0;
}

int receive_message(
    int socket_fd,
    struct sockaddr_in socket_address,
    char* buff
) {
    socklen_t address_len = sizeof(socket_address);

    int recv_result = recvfrom(
        socket_fd,
        buff,
        sizeof(buff),
        0,
        (struct sockaddr *)&socket_address,
        &address_len
    );

    if (recv_result < 0) {
        printf("Failed to receive a message from client\n");
        return -1;
    }

    return recv_result;
}

int main(void) {
    // Create UDP socket
    int socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_desc < 0) {
        printf("Failed to create a socket\n");
        return -1;
    }

    // Set IP and Port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    server_addr.sin_port = htons(SERVER_PORT);

    char server_msg_buff[2000];

//    for (int i = 0; i < SECRET_NUMBER; i++) {
    int i = 20;

    while (1) {
        char* msg_to_send = malloc(sizeof(char) * 2);
        sprintf(msg_to_send, "%d", i);

        send_message(socket_desc, server_addr, msg_to_send);

        // Receive a message from client
        memset(server_msg_buff, '\0', sizeof(server_msg_buff));
        receive_message(socket_desc, client_addr, server_msg_buff);

        printf("%s\n", server_msg_buff);

        if (strcmp(server_msg_buff, "LESS") == 0) {
            i++;
        } else if (strcmp(server_msg_buff, "MORE") == 0) {
            i--;
        } else if (strcmp(server_msg_buff, "WIN") == 0) {
            break;
        }
    }

    close(socket_desc);

    return 0;
}
