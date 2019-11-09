cmake_minimum_required(VERSION 3.14)
project(Rubezh C)

set(CMAKE_FLAGS "clang++ -std=c++11 -Wall -Werrr -pedantic")

include_directories(${PROJECT_SOURCE_DIR})
add_executable(task2Main main.c task2.c)