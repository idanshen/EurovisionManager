cmake_minimum_required(VERSION 3.13)
project(eurovision.c C)

set(CMAKE_C_STANDARD 99)

<<<<<<< Updated upstream
add_executable(Eurovision eurovision.c eurovision.h map.h map.c main.c judge.h judge.c state.h state.c list.h libmtm.a eurovisionTests.c eurovisionTests.h test_utilities.h)
target_link_libraries(Eurovision ${CMAKE_SOURCE_DIR}/libmtm.a)

=======
add_executable(Eurovision eurovision.c eurovision.h map.h map.c main.c judge.h
        judge.c state.h state.c list.h libmtm.a)

#set(LIBS_DIR "/cygdrive/c/Users/User/Documents/Spring2019/MATAM/HW1/Eurovision/EurovisionManager/")
#add_library(libmtm.a STATIC Eurovision set.h list.h)
#find_library("libmtm.a" /cygdrive/c/Users/User/Documents/Spring2019/MATAM/HW1/Eurovision/EurovisionManager/)
#target_link_libraries(Eurovision libmtm.a)
>>>>>>> Stashed changes
