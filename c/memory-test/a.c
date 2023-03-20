//
// Created by void on 3/18/23.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


void f1(int *a) {
    // If stored in Stack, value is copied and then changed,
    // If stored in Heap, value is changed itself
    (*a)++;
    printf("%d\n", *a);
}


int main(int argn, char** argv) {
    int a = 10;  // Allocated in Stack
    int *aa = &a; // Should be also allocated in stack
//    int *aa = malloc(sizeof(int));
//    *aa = 4;

    f1(aa);
    printf("%d\n", *aa);
}