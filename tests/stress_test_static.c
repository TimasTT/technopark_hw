#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <fcntl.h>

#include "file_utils_static.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        return 0;
    }
    int start = clock();
    //printf("%s %s %s\n", argv[1], argv[2], argv[3]);
    set_processes_amount(atoi(argv[1]));
    set_file_size_amount(atoi(argv[2]));
    //fill_file(argv[3], file_s);

    int* dif_cnt = malloc((processes + 1)* sizeof(int));
    if (dif_cnt == NULL) {
        return -1;
    }

    file_bytes_check(argv[3], file_s, dif_cnt, processes);
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
    return 0;
}
