cmake_minimum_required(VERSION 3.5)
project(Parser)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES main.c)
add_executable(Parser ${SOURCE_FILES})