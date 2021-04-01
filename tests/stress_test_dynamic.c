#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <fcntl.h>
#include <dlfcn.h>


//#include "file_utils_dynamic.h"

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
    //printf("%s %s %s\n", argv[1], argv[2], argv[3]);

    void (*set_processes_amount)(int);
    set_processes_amount = dlsym(handle, "set_processes_amount");
    (*set_processes_amount)(atoi(argv[1]));

    void (*set_file_size_amount)(int);
    set_file_size_amount = dlsym(handle, "set_file_size_amount");
    (*set_file_size_amount)(atoi(argv[2]));

    int file_s = *(int *) dlsym(handle, "file_s");
//    void (*fill_file)(const char *, int);
//    fill_file = dlsym(handle, "fill_file");
//    (*fill_file)(argv[3], file_s);

    int processes = *(int *) dlsym(handle, "processes");

    int *dif_cnt = malloc((processes + 1) * sizeof(int));
    if (dif_cnt == NULL) {
        return -1;
    }

    void (*file_bytes_check)(const char *, int, int *, int);
    file_bytes_check = dlsym(handle, "file_bytes_check");
    (*file_bytes_check)(argv[3], file_s, dif_cnt, processes);
    int end = clock();

    dif_cnt[processes] = end - start;

    int fd_res = open("result", O_RDWR);
    if (fd_res < 0) {
        free(dif_cnt);
        return -1;
    }

    /*for (int k = 0; k < processes + 1; k++) {
        printf("%i ", dif_cnt[k]);
    }
    printf("\n");*/

    int size = (processes + 1) * sizeof(int);
    if (write(fd_res, dif_cnt, (processes + 1) * sizeof(int)) != size) {
        free(dif_cnt);
        return -1;
    }

    free(dif_cnt);
    //printf("%i\n", end - start);
    dlclose(handle);
    return 0;
}
