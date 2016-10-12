cmake_minimum_required(VERSION 3.6)
project(jhsolver2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp datastructures.cpp datastructures.h kml_handling.cpp kml_handling.h global.cpp global.h)
add_executable(jhsolver2 ${SOURCE_FILES})