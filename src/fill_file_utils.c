#include <file_utils.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <zconf.h>

const char random_data[] = "/dev/urandom";
const int max_urandom_size = 33554431;

errors fill_file_random(const char* filename, int file_size) {
    if  (filename == NULL) {
        return ERR_INVALID_INPUT;
    }

    int fd_random_data = open(random_data, O_RDONLY);
    if (fd_random_data < 0) {
        return ERR_BAD_FILE_OPENING;
    }

    int fd_filling_file = open(filename, O_RDWR);
    if (fd_filling_file < 0) {
        close(fd_random_data);
        return ERR_BAD_FILE_OPENING;
    }

    char* buf = calloc((file_size + 1), sizeof(char));
    if (!buf) {
        close(fd_random_data);
        close(fd_filling_file);
        return ERR_BAD_ALLOC;
    }
    memset(buf, '0', file_size);

    ssize_t i = read(fd_random_data, buf, file_size);
    if ((i != file_size && i < max_urandom_size) || i == -1) {
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