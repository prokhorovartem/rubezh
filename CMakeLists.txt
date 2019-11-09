cmake_minimum_required(VERSION 3.14)
project(Rubezh C)

set(CMAKE_FLAGS "clang++ -std=c++11 -Wall -Werrr -pedantic")

add_executable(Rubezh main.c)