cmake_minimum_required(VERSION 3.15)
project(conc_skiplist)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES
        source/*.h
        )
find_package(Threads REQUIRED)

add_executable(cSkipList main.cpp ${SOURCES} )
target_link_libraries (cSkipList Threads::Threads)