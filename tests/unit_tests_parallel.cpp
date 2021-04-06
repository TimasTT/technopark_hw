#include <gtest/gtest.h>
#include <string>
#include <stdlib.h>
#include <sys/mman.h>

extern "C" {
    #include "file_utils.h"
}

TEST(FileUtilsTest, FillFile) {
    int file_size = 10 * 1024;
    const char* filename = "test_file";
    ASSERT_EQ(fill_file_random(filename, file_size), SUCCESSFUL);
}

TEST(FileUtilsTest, FillFileInvalidInput) {
    int file_size = 10 * 1024;
    ASSERT_EQ(fill_file_random(NULL, file_size), ERR_INVALID_INPUT);
}

TEST(FileUtilsTest, FillFileInvalidFilename) {
    int file_size = 10 * 1024;
    const char* filename = "nonexistantfilename";
    ASSERT_EQ(fill_file_random(filename, file_size), ERR_BAD_FILE_OPENING);
}

TEST(FileUtilsTest, CheckBytesProcessesInFileInvalidInput) {
    int file_size = 10 * 1024;

    ASSERT_EQ(file_bytes_check(NULL, file_size, NULL, 2), ERR_INVALID_INPUT);
}

TEST(FileUtilsTest, CheckBytesProcessesInFileInvalidFilename) {
    int file_size = 10 * 1024;
    const char* filename = "nonexistantfilename";

    int *a = (int*)malloc(2 * sizeof(int));
    ASSERT_EQ(file_bytes_check(filename, file_size, a, 2), ERR_BAD_FILE_OPENING);
    free(a);
}

TEST(FileUtilsTest, CheckBytesIn100mbFileWithProcesses) {
    int file_size = 100 * 1024 * 1024;
    int processes = 10;
    const char* filename = "my_file";
    ASSERT_EQ(fill_file_random(filename, file_size), SUCCESSFUL);

    int *a = (int*)malloc(processes * sizeof(int));
    ASSERT_EQ(file_bytes_check(filename, file_size, a, processes), SUCCESSFUL);
    free(a);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

