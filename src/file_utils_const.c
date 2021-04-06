#include <fcntl.h>
#include <sys/mman.h>
#include <zconf.h>
#include "file_utils.h"

errors bytes_check(const char* checking_region, int file_size, int dif) {
    if (checking_region == NULL) {
        return ERR_INVALID_INPUT;
    }
    int cnt = 0;
    for (int i = 0; i < file_size - 1; ++i) {
        if (checking_region[i] - checking_region[i + 1] == dif ||
            checking_region[i + 1] - checking_region[i] == dif) {
            cnt++;
        }
    }

    return cnt;
}

errors file_bytes_check(const char* filename, int file_size, int* dif_cnt, int cnt) {
    if (dif_cnt == NULL || filename == NULL) {
        return ERR_INVALID_INPUT;
    }

    int fd_checking_file = open(filename, O_RDONLY);
    if (fd_checking_file < 0) {
        return ERR_BAD_FILE_OPENING;
    }

    char* checking_region = mmap(NULL, file_size, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_POPULATE, fd_checking_file, 0);
    if (checking_region == MAP_FAILED) {
        close(fd_checking_file);
        return ERR_BAD_FILE_OPENING;
    }

    for (int i = 0; i < cnt; ++i) {
        int tmp = bytes_check(checking_region, file_size, i);
        if (tmp < 0) {
            return tmp;
        } else {
            dif_cnt[i] = tmp;
        }
    }

    if (munmap(checking_region, file_size) != 0) {
        return ERR_MUNMAP;
    }

    close(fd_checking_file);
    return SUCCESSFUL;
}