project(cargparser)
cmake_minimum_required(VERSION 3.17)
include_directories(include)
file(GLOB INCLUDES include/*.h)
file(GLOB SOURCES src/*.c)
add_executable(test test/main.c ${INCLUDES} ${SOURCES})

