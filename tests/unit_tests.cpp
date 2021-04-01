#include <gtest/gtest.h>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

extern "C" {
    #include "file_utils.h"
}

TEST(FileUtilsTest, FillFile) {
    set_file_size_amount(10 * 1024);
    const char* filename = "test_file";
    ASSERT_EQ(fill_file(filename, file_s), SUCCESSFUL);
}

TEST(FileUtilsTest, FillFileInvalidInput) {
    set_file_size_amount(10 * 1024);
    ASSERT_EQ(fill_file(NULL, file_s), ERR_INVALID_INPUT);
}

TEST(FileUtilsTest, FillFileInvalidFilename) {
    set_file_size_amount(10 * 1024);
    const char* filename = "nonexistantfilename";
    ASSERT_EQ(fill_file(filename, file_s), ERR_BAD_FILE_OPENING);
}

TEST(FileUtilsTest, CheckBytes) {
    char* str = (char* )malloc(2 * sizeof(char));
    str[0] = '8';
    str[1] = '~';

    ASSERT_EQ(bytes_check(str, 2, 70), 1);
    free(str);
}

TEST(FileUtilsTest, CheckBytesInvalidInput) {
    ASSERT_EQ(bytes_check(NULL, 2, 70), ERR_INVALID_INPUT);
}

TEST(FileUtilsTest, CheckBytesInFile) {
    set_file_size_amount(10 * 1024);
    const char* filename = "my_file";
    ASSERT_EQ(fill_file(filename, file_s), SUCCESSFUL);

    int *a = (int*)malloc(2 * sizeof(int));
    ASSERT_EQ(file_bytes_check(filename, file_s, a, 2), SUCCESSFUL);
    free(a);
}

TEST(FileUtilsTest, CheckBytesInFileInvalidInput) {
    set_file_size_amount(10 * 1024);

    ASSERT_EQ(file_bytes_check(NULL, file_s, NULL, 2), ERR_INVALID_INPUT);
}

TEST(FileUtilsTest, CheckBytesInFileInvalidFilename) {
    set_file_size_amount(10 * 1024);
    const char* filename = "nonexistantfilename";

    int *a = (int*)malloc(2 * sizeof(int));
    ASSERT_EQ(file_bytes_check(filename, file_s, a, 2), ERR_BAD_FILE_OPENING);
    free(a);
}

TEST(FileUtilsTest, CheckBytesIn100mbFile) {
    set_file_size_amount(100 * 1024 * 1024);
    set_processes_amount(10);
    const char* filename = "my_file";
    ASSERT_EQ(fill_file(filename, file_s), SUCCESSFUL);

    int *a = (int*)malloc(processes * sizeof(int));
    ASSERT_EQ(file_bytes_check(filename, file_s, a, processes), SUCCESSFUL);
    free(a);
}

TEST(FileUtilsTest, CheckRunningProcesses) {
    set_file_size_amount(100 * 1024 * 1024);
    set_processes_amount(10);
    int fd_checking_file = open("my_file", O_RDONLY);

    char* checking_region = static_cast<char *>(mmap(NULL, file_s, PROT_READ | PROT_WRITE,
                                                     MAP_PRIVATE | MAP_POPULATE, fd_checking_file, 0));


    char* result_region = static_cast<char *>(mmap(NULL, processes * sizeof(int), PROT_READ | PROT_WRITE,
                                                   MAP_SHARED | MAP_ANONYMOUS, -1, 0));

    int bytes_for_one_calculation = 4;
    ASSERT_EQ(running_processes(result_region, checking_region, file_s, processes, bytes_for_one_calculation),
              SUCCESSFUL);
}

TEST(FileUtilsTest, CheckRunningProcessesInvalidInput) {
    set_file_size_amount(100 * 1024 * 1024);
    set_processes_amount(10);

    int bytes_for_one_calculation = 4;
    ASSERT_EQ(running_processes(NULL, NULL, file_s, processes, bytes_for_one_calculation),
              ERR_INVALID_INPUT);
}

TEST(FileUtilsTest, CheckBytesProcessesInFileInvalidInput) {
    set_file_size_amount(10 * 1024);

    ASSERT_EQ(file_bytes_check_processes(NULL, file_s, NULL, 2), ERR_INVALID_INPUT);
}

TEST(FileUtilsTest, CheckBytesProcessesInFileInvalidFilename) {
    set_file_size_amount(10 * 1024);
    const char* filename = "nonexistantfilename";

    int *a = (int*)malloc(2 * sizeof(int));
    ASSERT_EQ(file_bytes_check_processes(filename, file_s, a, 2), ERR_BAD_FILE_OPENING);
    free(a);
}

TEST(FileUtilsTest, CheckBytesIn100mbFileWithProcesses) {
    set_file_size_amount(100 * 1024 * 1024);
    set_processes_amount(10);
    const char* filename = "my_file";
    ASSERT_EQ(fill_file(filename, file_s), SUCCESSFUL);

    int *a = (int*)malloc(processes * sizeof(int));
    ASSERT_EQ(file_bytes_check_processes(filename, file_s, a, processes), SUCCESSFUL);
    free(a);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
