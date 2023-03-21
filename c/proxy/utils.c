//
// Created by void on 3/21/23.
//

#include "lib_tcp.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

int log_func(int func_result, char* log_msg) {
    printf("%s: ", log_msg);

    if (func_result < 0) { // == -1
        printf("FAILED (%d)\n", func_result);
        return -1;
    } else {
        printf("OK (%d)\n", func_result);
        return func_result;
    }
}

void write_to_file(char* fpath, char* string) {
    int fd = open(fpath, O_CREAT | O_WRONLY, 0666);

    printf("Result string to file: %s\n", string);

    write(fd, string, strlen(string));
    close(fd);
}

void wait_interrupt(char* msg_print) {
    printf("%s", msg_print);
    char *q1 = malloc(sizeof(char) * 10);

    scanf("%s", q1);
}


int get_server_port(int argn, char** argv) {
    if (argn > 1) {
        return (int) strtol(argv[1], NULL, 10);
    } else {
        printf("setting SERVER_PORT to default: %d\n", DEFAULT_SERVER_PORT);
        return DEFAULT_SERVER_PORT;
    }
}

void setup_server_addr(struct sockaddr_in* server_addr, int SERVER_PORT) {
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = inet_addr(SERVER_IP_ADDR);
    server_addr->sin_port = htons(SERVER_PORT);
}