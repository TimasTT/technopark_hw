#include <gtest/gtest.h>
#include <string>
#include <stdlib.h>

extern "C" {
    #include "file_utils_static.h"
}

TEST(FileUtilsTest, FillFile) {
    set_file_size_amount(10 * 1024);
    const char* filename = "test_file";
    ASSERT_EQ(fill_file(filename, file_s), SUCCESSFUL);
}

TEST(FileUtilsTest, CheckBytes) {
    char* str = (char* )malloc(2 * sizeof(char));
    str[0] = '8';
    str[1] = '~';

    ASSERT_EQ(bytes_check(str, 2, 70), 1);
    free(str);
}

TEST(FileUtilsTest, CheckBytesInFile) {
    set_file_size_amount(10 * 1024);
    const char* filename = "my_file";
    ASSERT_EQ(fill_file(filename, file_s), SUCCESSFUL);

    int *a = (int*)malloc(2 * sizeof(int));
    ASSERT_EQ(file_bytes_check(filename, file_s, a, 2), SUCCESSFUL);
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
