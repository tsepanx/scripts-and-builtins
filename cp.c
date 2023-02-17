#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define ARGS_CNT 3

int to_int(char ch) { return (int) ch - '0'; }

void add_to_file(char* fpath, char* string) {
    int fd = open(fpath, O_APPEND);

    write(fd, string, strlen(string));
    close(fd);
}

unsigned int copy(char* from_path, char* to_path, unsigned int chunk_size, int logging_flag) {
    printf("CHUNK SIZE: %10d | ", chunk_size);

    unsigned int file_size = 0;

    int from_fd = open(from_path, O_RDONLY);
    int to_fd = open(to_path, O_CREAT | O_WRONLY, 0666);

    char *buff = malloc(sizeof(char) * chunk_size);
    while (1) {
        int read_size = (int) read(from_fd, buff, chunk_size);

        if (logging_flag) {
            printf("\n--- CHUNK ---\n");
            for (int i = 0; i < read_size; ++i) {
                printf("    Byte: '%c' (%d)\n", buff[i], (int) buff[i]);
            }
        }

        if (read_size == -1) { printf("Error reading file\n"); exit(1); }

        if (read_size == 0) {
            if (logging_flag) printf("\n--- EOF ---\n");
            break;
        }

        file_size += read_size;

        int write_status = (int) write(to_fd, buff, read_size);
        if (write_status == -1) {
            printf("Error writing to file.\n");
            exit(1);
        }
    }

    close(from_fd);
    close(to_fd);

    return file_size;
}

int main(int argn, char * argv[]) {
    if (argn != ARGS_CNT + 1) {
        printf("Wrong args count\n");
        exit(1);
    }

    char* from_path = argv[1];
    char* to_path = argv[2];
//    int chunk_size = to_int(*argv[3]);

    printf("FROM: %s | TO: %s\n", from_path, to_path);


    for (int i = 1; i < 2; ++i) {
        clock_t start = clock();

        int chunk_size = i * 128;
        size_t size_in_bytes = copy(from_path, to_path, chunk_size, 1);

        unsigned int ticks = clock() - start;
        float seconds = ((float) ticks)/CLOCKS_PER_SEC;
        long int speed = (int) (((float) size_in_bytes) / seconds);
        printf("SECONDS: %.4f, SPEED: %.20zu\n", seconds, speed);
    }
}



