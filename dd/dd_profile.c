#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "dd.c"

int main(int argn, char * argv[]) {
    if (argn != ARGS_CNT + 1) {
        printf("Wrong args count\n");
        exit(1);
    }

    char* from_path = argv[1];
    char* to_path = argv[2];

    for (int i = 1; i < 2; ++i) {
        clock_t start = clock();

        int chunk_size = i * 128;
        size_t size_in_bytes = copy(from_path, to_path, chunk_size);

        unsigned int ticks = clock() - start;
        float seconds = ((float) ticks)/CLOCKS_PER_SEC;
        long int speed = (int) (((float) size_in_bytes) / seconds);
        printf("SECONDS: %.4f, SPEED: %.20zu\n", seconds, speed);
    }

    printf("FROM: %s | TO: %s\n", from_path, to_path);
}



