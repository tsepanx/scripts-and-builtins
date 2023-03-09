/*
 * Original program
 * https://github.com/coreutils/coreutils/blob/master/src/dd.c
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

    char* buff = malloc(sizeof(char) * chunk_size);
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

    char* from_path = argv[1];
    char* to_path = argv[2];

    int chunk_size;
    if (argn >= 3 + 1) {
        chunk_size = to_int(*argv[3]);
    } else {
        chunk_size = 2 << 13; // 8096
    }

    int logging_flag;
    if (argn >= 4 + 1) {
        logging_flag = to_int(*argv[4]);
    } else {
        logging_flag = 1;
    }

    copy(from_path, to_path, chunk_size, logging_flag);

    printf("FROM: %s | TO: %s\n", from_path, to_path);
}



