cmake_minimum_required(VERSION 3.6)
project(technopark_hw1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDART_REQURED ON)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
#[[set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)]]
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

add_library(Mail STATIC src/mail_check_spam.c
                        src/custom_string.c)
target_include_directories(Mail PUBLIC include)

add_executable(prog ./src/main.c)
target_link_libraries(prog Mail)

add_subdirectory(thirdparty/gtest)
find_package(Threads)
add_executable(test tests/tests.cpp)
target_link_libraries(test Mail gtest ${CMAKE_THREAD_LIBS_INIT})


