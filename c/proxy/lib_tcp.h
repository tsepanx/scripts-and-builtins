//
// Created by void on 3/20/23.
//

#ifndef C_LIB_TCP_H
#define C_LIB_TCP_H

#include <arpa/inet.h>

extern void write_to_file(char* path, char* buf);
extern int create_tcp_socket_fd();
extern int send_message(int socket_fd, char *buf);
extern int receive_message(int socket_fd, char* buf, int size);
extern int connect_to_addr(int socket_fd, struct sockaddr_in server_addr);
int log_func(int func_result, char* log_msg);

#endif //C_LIB_TCP_H
