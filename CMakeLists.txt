cmake_minimum_required(VERSION 3.20)
project(cedis)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    src/main.cpp
)

add_executable(cedis src/main.cpp)