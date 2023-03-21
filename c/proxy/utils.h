//
// Created by void on 3/21/23.
//

#ifndef C_UTILS_H
#define C_UTILS_H

#include <arpa/inet.h>

#define MESSAGE_SIZE 2000
#define DEFAULT_SERVER_PORT 2022

extern int log_func(int func_result, char* log_msg);
extern void write_to_file(char* path, char* buf);
extern void wait_interrupt(char* msg_print);
extern int get_server_port(int argn, char** argv);
extern void setup_server_addr(struct sockaddr_in* server_addr, int SERVER_PORT);

#endif //C_UTILS_H
