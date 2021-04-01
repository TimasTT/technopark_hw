#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "file_utils.h"

int main() {
    set_file_size_amount(100 * 1024 * 1024);
    const char* filename = "my_file";

    fill_file(filename, file_s);

    return 0;
}
