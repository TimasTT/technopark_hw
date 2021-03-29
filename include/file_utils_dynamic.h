#ifndef INCLUDE_FILE_UTILS_DYNAMIC_H_
#define INCLUDE_FILE_UTILS_DYNAMIC_H_

int processes;
void set_processes_amount(int amount);

int file_s;
void set_file_size_amount(int amount);

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

errors bytes_check(const char* filename, int file_size, int dif);

errors fill_file(const char* filename, int file_size);

errors file_bytes_check(const char* filename, int file_size, int* dif_cnt, int cnt);

#endif  // INCLUDE_FILE_UTILS_DYNAMIC_H_
