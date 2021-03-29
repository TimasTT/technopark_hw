cmake_minimum_required(VERSION 3.6)
project(technopark_hw1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDART_REQURED ON)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -Wextra -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror -Wextra -fprofile-arcs -ftest-coverage")
#[[set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)]]
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

add_library(FileUtilsStatic STATIC include/file_utils_static.h src/file_utils_static.c)
target_include_directories(FileUtilsStatic PUBLIC include)


add_library(FileUtilsDynamic SHARED include/file_utils_dynamic.h src/file_utils_dynamic.c)
target_include_directories(FileUtilsDynamic PUBLIC include)

#realisation with static library
add_executable(prog ./src/main.c)
target_link_libraries(prog FileUtilsStatic)

#realisation with dynamic library
#[[add_executable(prog ./src/main.c)
target_link_libraries(prog FileUtilsDynamic)]]

add_subdirectory(thirdparty)
find_package(Threads)

#unit-tests
add_executable(test_s tests/unit_tests_static.cpp)
target_link_libraries(test_s FileUtilsStatic gtest ${CMAKE_THREAD_LIBS_INIT})

add_executable(test_d tests/unit_tests_dynamic.cpp)
target_link_libraries(test_d FileUtilsDynamic gtest ${CMAKE_THREAD_LIBS_INIT})

#stress-tests
add_executable(stress_test_controller tests/stress_tests_controller.c)
#target_link_libraries(stress_test FileUtilsStatic gtest ${CMAKE_THREAD_LIBS_INIT})

add_executable(stress_test_d tests/stress_test_dynamic.c)
target_link_libraries(stress_test_d FileUtilsDynamic)

add_executable(stress_test_s tests/stress_test_static.c)
target_link_libraries(stress_test_s FileUtilsStatic)