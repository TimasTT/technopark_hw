#ifndef INCLUDE_FILE_UTILS_STATIC_H_
#define INCLUDE_FILE_UTILS_STATIC_H_

typedef enum {
    ERR_BAD_FILE_READING,
    ERR_BAD_FILE_WRITHING,
    ERR_BAD_FILE_OPENING,
    ERR_BAD_ALLOC,
    ERR_INVALID_INPUT,
    ERR_BAD_FORK,
    ERR_MUNMAP,

    SUCCESSFUL = 0
} errors;

errors fill_file_random(const char* filename, int file_size);

errors file_bytes_check(const char* filename, int file_size, int* dif_cnt, int cnt);

#endif  // INCLUDE_FILE_UTILS_STATIC_H_
