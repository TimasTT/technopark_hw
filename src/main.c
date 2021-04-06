#include "file_utils.h"

int main() {
    int file_size = 10 * 1024 * 1024;
    const char* filename = "my_file";

    fill_file_random(filename, file_size);

    return 0;
}
