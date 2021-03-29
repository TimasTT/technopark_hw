#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zconf.h>
#include <sys/mman.h>
#include <wait.h>
#include <math.h>
#include <string.h>

#include "file_utils_dynamic.h"

void set_processes_amount(int amount) {
    processes = amount;
}

void set_file_size_amount(int amount) {
    file_s = amount;
}

errors bytes_check(const char* checking_region, int file_size, int dif) {
    int cnt = 0;
    for (int i = 0; i < file_size - 1; ++i) {
        if (checking_region[i] - checking_region[i + 1] == dif ||
            checking_region[i + 1] - checking_region[i] == dif) {
            cnt++;
        }
    }

    return cnt;
}

errors fill_file(const char* filename, int file_size) {
    int fd_random_data = open("/dev/urandom", O_RDONLY);
    if (fd_random_data < 0) {
        return ERR_BAD_FILE_OPENING;
    }

    int fd_filling_file = open(filename, O_RDWR);
    if (fd_filling_file < 0) {
        close(fd_random_data);
        return ERR_BAD_FILE_OPENING;
    }

    char *buf = malloc(file_size);
    if (!buf) {
        close(fd_random_data);
        close(fd_filling_file);
        return ERR_BAD_ALLOC;
    }
    memset(buf, '0', file_size);

    ssize_t i = read(fd_random_data, buf, file_size);
    if (i != file_size && i < 33554431) {
        free(buf);
        close(fd_random_data);
        close(fd_filling_file);
        return ERR_BAD_FILE_READING;
    }

    if (write(fd_filling_file, buf, file_size) != file_size) {
        free(buf);
        close(fd_random_data);
        close(fd_filling_file);
        return ERR_BAD_FILE_WRITHING;
    }

    free(buf);
    close(fd_filling_file);
    close(fd_random_data);
    return SUCCESSFUL;
}

errors file_bytes_check(const char* filename, int file_size, int* dif_cnt, int cnt) {
    if (dif_cnt == NULL) {
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

    char* result_region = mmap(NULL, cnt * sizeof(int), PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (!result_region) {
        close(fd_checking_file);
        return ERR_BAD_FILE_OPENING;
    }

    int* pid_list = malloc(cnt * sizeof(int));
    int pid = -1, status, pid_list_iter = 0;
    int i;
    for (i = 0; i < cnt; ++i) {
        if (pid > 0) {
            pid_list[pid_list_iter++] = pid;
        } else if (pid == 0) {
            int tmp = bytes_check(checking_region, file_size, i - 1);
            for (int k = 0; k < 4; ++k) {
                result_region[i * 4 - k] = tmp % 128;
                tmp /= 128;
            }
            if (munmap(checking_region, file_size) != 0) {
                free(pid_list);
                close(fd_checking_file);
                exit(0);
            }

            free(pid_list);
            close(fd_checking_file);
            exit(0);
        }
        pid = fork();
    }

    if (pid > 0) {
        pid_list[pid_list_iter++] = pid;
    } else {
        int tmp = bytes_check(checking_region, file_size, i - 1);
        for (int k = 0; k < 4; ++k) {
            result_region[i * 4 - k] = tmp % 128;
            tmp /= 128;
        }
        if (munmap(checking_region, file_size) != 0) {
            free(pid_list);
            close(fd_checking_file);
            exit(0);
        }

        free(pid_list);
        close(fd_checking_file);
        exit(0);
    }

    pid_list_iter = 0;
    do {
        pid_t waited_pid = waitpid(pid_list[pid_list_iter], &status, WNOHANG);

        if (waited_pid < 0) {
            if (munmap(checking_region, file_size) != 0) {
                if (munmap(result_region, 20) != 0) {
                    free(pid_list);
                    close(fd_checking_file);
                    return ERR_MUNMAP;
                }
                free(pid_list);
                close(fd_checking_file);
                return ERR_MUNMAP;
            }

            if (munmap(result_region, 20) != 0) {
                free(pid_list);
                close(fd_checking_file);
                return ERR_MUNMAP;
            }
            free(pid_list);
            close(fd_checking_file);
            return ERR_BAD_FORK;
        }

        if (waited_pid) {
            pid_list_iter++;
        }
    } while (pid_list_iter != cnt);

    free(pid_list);

    int k = 1;
    for (int j = 0; j < cnt; ++j) {
        dif_cnt[j] = result_region[k * 4] + result_region[k * 4 - 1] * 128
                     + result_region[k * 4 - 2] * pow(128, 2) + result_region[k * 4 - 3] * pow(128, 3);
        k++;
    }

    if (munmap(checking_region, file_size) != 0) {
        if (munmap(result_region, 20) != 0) {
            close(fd_checking_file);
            return ERR_MUNMAP;
        }
        close(fd_checking_file);
        return ERR_MUNMAP;
    }

    if (munmap(result_region, 20) != 0) {
        close(fd_checking_file);
        return ERR_MUNMAP;
    }

    close(fd_checking_file);

    return SUCCESSFUL;
}

