//
// Created by void on 3/21/23.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <grp.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>

#define STR_SIZE 1024

#define USR_WRITE_BIT 7
#define GRP_WRITE_BIT 4
#define ALL_WRITE_BIT 1

char* exclude_entries[] = { "/sys", "/proc" };

struct user_input {
    char* username;
    char* groupname;
    char* path;
};

int path_writeable(struct stat path_stat, struct user_input ui) {
    struct group* grp = getgrnam(ui.groupname);
    struct passwd* usr = getpwnam(ui.username);

    unsigned int usr_uid = usr->pw_uid;
    unsigned int grp_gid = grp->gr_gid;

    unsigned int access_rights = path_stat.st_mode & ACCESSPERMS;

    // file belongs to user
    if (path_stat.st_uid == usr_uid) {
        // writeable by user
        if (access_rights & (1 << USR_WRITE_BIT)) {
            return 1;
        }
    }

    // file belongs to group
    if (path_stat.st_gid == grp_gid) {
        // writeable by group
        if (access_rights & (1 << GRP_WRITE_BIT)) {
            return 1;
        }
    }

    // writeable by all
    if (access_rights & (1 << ALL_WRITE_BIT)) {
        return 1;
    }

    return 0;
}

int traverse(char *path, struct user_input ui) {
    int exclude_arr_len = sizeof(exclude_entries) / sizeof(exclude_entries[0]);
    int matches_exclude = 0;
    for (int i = 0; i < exclude_arr_len; ++i) {
        if (strcmp(path, exclude_entries[i]) == 0) {
            matches_exclude = 1;
        }
    }
    if (matches_exclude) { return 0; }

    struct stat path_stat;
    char err_msg[STR_SIZE];

    if (lstat(path, &path_stat) == -1) {
        perror(path);
        return -1;
    }

    int is_dir = S_ISDIR(path_stat.st_mode);
    int is_writeable = path_writeable(path_stat, ui);

    if (is_writeable) {
        // printf("%o %b ", access_rights, access_rights);
        char* prefix = is_dir ? "d" : "f";
        printf("%s %s\n", prefix, path);
    }

    if (is_dir == 1) {
        DIR *dir = opendir(path);
        if (!dir) {
            sprintf(err_msg, "dir %s", path);
            perror(err_msg);
            return -1;
        }

        struct dirent *dirent_i;
        while ((dirent_i = readdir(dir)) != NULL) {
            if (strcmp(dirent_i->d_name, ".") == 0 || strcmp(dirent_i->d_name, "..") == 0) {
                continue;
            }

            char full_path[STR_SIZE];
            // If path == "/", replace it with "" to avoid duplicate slashes "//sub-path"
            snprintf(
                full_path,
                sizeof(full_path),
                "%s/%s", (strcmp(path, "/") == 0 ? "" : path),
                dirent_i->d_name
            );

            // Recursively dive into sub-dir
            traverse(full_path, ui);
        }
        closedir(dir);
    }
    return 0;
}


int main(int argn, char** argv) {
    if (geteuid() != 0) {
        fprintf(stderr, "This program must be run as root\n");
        return 1;
    }

    struct user_input ui;

    if (argn == 4) {
        ui.username = argv[1];
        ui.groupname = argv[2];

        ui.path = malloc(STR_SIZE);
        realpath(argv[3], ui.path);
    } else {
        printf("Usage:\n$ %s USERNAME GROUPNAME PATH\n", argv[0]);
        return -1;
    }

    struct group* grp = getgrnam(ui.groupname);
    if (grp == NULL) {
        fprintf(stderr, "No such group: %s\n", ui.groupname);
        return -1;
    }

    struct passwd* usr = getpwnam(ui.username);
    if (usr == NULL) {
        fprintf(stderr, "No such user: %s\n", ui.username);
        return -1;
    }

    return traverse(ui.path, ui);
}
