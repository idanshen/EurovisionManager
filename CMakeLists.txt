cmake_minimum_required(VERSION 3.13)
project(eurovision.c C)

set(CMAKE_C_STANDARD 99)

add_executable(Eurovision eurovision.c eurovision.h map.h main.c)