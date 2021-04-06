#include <stdlib.h>
#include <sys/mman.h>
#include <wait.h>
#include <zconf.h>
#include <math.h>
#include <fcntl.h>
#include <stdio.h>

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

errors running_processes(char* result_region, char* checking_region, int file_size, int cnt,
                         int bytes_for_one_calculation) {
    if (result_region == NULL || checking_region == NULL) {
        return ERR_INVALID_INPUT;
    }

    int* pid_list = malloc(cnt * sizeof(int));
    if (pid_list == NULL) {
        return ERR_BAD_ALLOC;
    }
    int status, pid_list_iter = 0;

    for (int i = 0; i < cnt; ++i) {
        int pid = fork();

        if (pid > 0) {
            pid_list[pid_list_iter++] = pid;
        } else {
            if (i == cnt - 1) {
                free(pid_list);
                exit(0);
            }

            int tmp = bytes_check(checking_region, file_size, i);
            for (int k = 0; k < bytes_for_one_calculation; ++k) {
                result_region[(i + 1) * bytes_for_one_calculation - k] = tmp % 128;
                tmp /= 128;
            }
            if (munmap(checking_region, file_size) != 0) {
                free(pid_list);
                exit(0);
            }

            free(pid_list);
            exit(0);
        }

        if (i == cnt - 1) {
            int tmp = bytes_check(checking_region, file_size, i);
            for (int k = 0; k < bytes_for_one_calculation; ++k) {
                result_region[(i + 1) * bytes_for_one_calculation - k] = tmp % 128;
                tmp /= 128;
            }
            if (munmap(checking_region, file_size) != 0) {
                free(pid_list);
                exit(0);
            }
        }
    }

    pid_list_iter = 0;
    do {
        pid_t waited_pid = waitpid(pid_list[pid_list_iter], &status, WNOHANG);

        if (waited_pid < 0) {
            if (munmap(checking_region, file_size) != 0) {
                if (munmap(result_region, bytes_for_one_calculation * 4) != 0) {
                    free(pid_list);
                    return ERR_MUNMAP;
                }
                free(pid_list);
                return ERR_MUNMAP;
            }

            if (munmap(result_region, bytes_for_one_calculation * 4) != 0) {
                free(pid_list);
                return ERR_MUNMAP;
            }
            free(pid_list);
            return ERR_BAD_FORK;
        }

        if (waited_pid) {
            pid_list_iter++;
        }
    } while (pid_list_iter != cnt);

    free(pid_list);
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

    int bytes_for_one_calculation = 4;
    if(running_processes(result_region, checking_region, file_size, cnt, bytes_for_one_calculation) != SUCCESSFUL) {
        return ERR_BAD_FORK;
    }

    int k = 1;
    for (int j = 0; j < cnt; ++j) {
        dif_cnt[j] = result_region[k * bytes_for_one_calculation] + result_region[k * bytes_for_one_calculation - 1] * 128
                     + result_region[k * bytes_for_one_calculation - 2] * pow(128, 2)
                     + result_region[k * bytes_for_one_calculation - 3] * pow(128, 3);
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