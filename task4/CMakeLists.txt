cmake_minimum_required(VERSION 3.14.5)
project(thread_pool_master)

set(SOURCES main.cpp SafeQueue.h ThreadPool.h)

SET(CMAKE_CXX_FLAGS -pthread)

add_compile_options(
        -std=c++11
)

# main.cpp
add_executable(main ${HEADERS} ${SOURCES})
target_link_libraries(main)