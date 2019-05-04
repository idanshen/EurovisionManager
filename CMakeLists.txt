cmake_minimum_required(VERSION 3.13)
project(eurovision.c C)

set(CMAKE_C_STANDARD 99)

add_executable(Eurovision eurovision.c eurovision.h euroMap.h euroMap.c main.c judge.h
        judge.c state.h state.c list.h libmtm.a eurovisionTests.c set.h
        eurovisionTests.h test_utilities.h map_utils.c map_utils.h)
target_link_libraries(Eurovision ${CMAKE_SOURCE_DIR}/libmtm.a)


