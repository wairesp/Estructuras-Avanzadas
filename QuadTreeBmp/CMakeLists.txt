cmake_minimum_required(VERSION 3.15)
project(myonlyTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(bmp bmp.cpp bitmap.h bitmap.cpp)
