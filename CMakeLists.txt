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

add_library(FileUtilsStatic STATIC include/file_utils.h src/file_utils_const.c src/fill_file_utils.c)
target_include_directories(FileUtilsStatic PUBLIC include)

add_library(FileUtilsDynamic SHARED include/file_utils.h src/file_utils_parallel.c src/fill_file_utils.c)
target_include_directories(FileUtilsDynamic PUBLIC include)

#realisation with static library
add_executable(prog ./src/main.c)
target_link_libraries(prog FileUtilsDynamic)

#realisation with dynamic library
#[[add_executable(prog ./src/main.c)
target_link_libraries(prog FileUtilsDynamic)]]

add_subdirectory(thirdparty)
find_package(Threads)

#unit-tests
add_executable(u_test_static tests/unit_tests_const.cpp)
target_link_libraries(u_test_static FileUtilsStatic gtest ${CMAKE_THREAD_LIBS_INIT})

add_executable(u_test_dynamic tests/unit_tests_parallel.cpp)
target_link_libraries(u_test_dynamic FileUtilsDynamic gtest ${CMAKE_THREAD_LIBS_INIT})

#stress-tests
add_executable(stress_test_controller tests/stress_tests_controller.c)
#[[target_link_libraries(stress_test_controller)]]

add_executable(stress_test_d tests/stress_test_dynamic.c)
target_link_libraries(stress_test_d FileUtilsDynamic  ${CMAKE_DL_LIBS})

add_executable(stress_test_s tests/stress_test_static.c)
target_link_libraries(stress_test_s FileUtilsStatic)