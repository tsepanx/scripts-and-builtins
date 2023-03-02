cmake_minimum_required(VERSION 3.24)
project(c C)

set(CMAKE_C_STANDARD 17)

add_executable(c networks_labs/sockets-udp/server.c networks_labs/sockets-udp/client.c)
