#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>

int time_control(int* dif_cnt, int cnt, const char* lib_run) {
    if (system(lib_run)) {
        return -1;
    }

    int fd_res = open("result", O_RDWR);
    if (fd_res < 0) {
        return -1;
    }

    unsigned int i = read(fd_res, dif_cnt, (cnt + 1) * sizeof(int));
    if (i != (cnt + 1) * sizeof(int)) {
        close(fd_res);
        return -1;
    }

    close(fd_res);
    return 0;
}

int get_result_lib(int* dif_cnt, int cnt, const char* lib_run) {
    int lib_time = 0;
    for (int i = 0; i < 5; i++) {
        if (time_control(dif_cnt, cnt, lib_run)) {
            return -1;
        }
        lib_time += dif_cnt[cnt];
    }

    lib_time /= 5;
    return lib_time;
}

int main() {
    int processes = 10;
    int *dif_cnt_static_lib = malloc((processes + 1) * sizeof(int));
    if (dif_cnt_static_lib == NULL) {
        return -1;
    }
    int *dif_cnt_dynamic_lib = malloc((processes + 1) * sizeof(int));
    if (dif_cnt_dynamic_lib == NULL) {
        free(dif_cnt_static_lib);
        return -1;
    }

    int time_static_lib = get_result_lib(dif_cnt_static_lib, processes,
                                  "../cmake-build-debug/stress_test_s 10 104857600 my_file");
    if (time_static_lib == -1) {
        free(dif_cnt_dynamic_lib);
        free(dif_cnt_static_lib);
        return -1;
    }

    int time_dynamic_lib = get_result_lib(dif_cnt_dynamic_lib, processes,
                                   "../cmake-build-debug/stress_test_d 10 104857600 my_file");
    if (time_dynamic_lib == -1) {
        free(dif_cnt_dynamic_lib);
        free(dif_cnt_static_lib);
        return -1;
    }

    for (int i = 0; i < processes; ++i) {
        if (dif_cnt_dynamic_lib[i] != dif_cnt_static_lib[i]) {
            free(dif_cnt_static_lib);
            free(dif_cnt_dynamic_lib);
            return -1;
        }
    }

    printf("static lib time: %i; dynamic lib time: %i\n", time_static_lib, time_dynamic_lib);
    free(dif_cnt_dynamic_lib);
    free(dif_cnt_static_lib);
    return 0;
}


