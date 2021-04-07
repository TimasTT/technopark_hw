#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <fcntl.h>
#include <dlfcn.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        return 0;
    }

    void *handle;
    handle = dlopen("libFileUtilsDynamic.so", RTLD_LAZY);
    if (!handle) {
        fputs(dlerror(), stderr);
        exit(1);
    }

    int start = clock();

    int processes = atoi(argv[1]);
    int file_s = atoi(argv[2]);

    int *dif_cnt = malloc((processes + 1) * sizeof(int));
    if (dif_cnt == NULL) {
        return 1;
    }

    void (*file_bytes_check)(const char *, int, int *, int);
    file_bytes_check = dlsym(handle, "file_bytes_check");
    (*file_bytes_check)(argv[3], file_s, dif_cnt, processes);
    int end = clock();

    dif_cnt[processes] = end - start;

    int fd_res = open("result", O_RDWR);
    if (fd_res < 0) {
        free(dif_cnt);
        return 2;
    }

    int size = (processes + 1) * sizeof(int);
    if (write(fd_res, dif_cnt, (processes + 1) * sizeof(int)) != size) {
        free(dif_cnt);
        return 3;
    }

    free(dif_cnt);
    dlclose(handle);
    return 0;
}
