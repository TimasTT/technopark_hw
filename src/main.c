#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "file_utils_static.h"
// #include "file_utils_dynamic.h"

int main() {
    set_file_size_amount(100 * 1024 * 1024);
    // set_processes_amount(50);
    const char* filename = "my_file";

    // int* dif_cnt_1 = malloc(processes * sizeof(int));

    fill_file(filename, file_s);
    /*int start = clock();
    file_bytes_check(filename, file_s, dif_cnt_1, processes);
    for (int j = 0; j < processes; ++j) {
        printf("%i ", dif_cnt_1[j]);
    }
    printf("\n");
    int stop = clock();
    printf("%i\n", stop - start);

    free(dif_cnt_1);*/
    return 0;
}
