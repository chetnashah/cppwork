cmake_minimum_required(VERSION 3.30)
project(mikeshahchap1)

set(CMAKE_CXX_STANDARD 17)

add_executable(mikeshahchap1 src/main.cpp src/ppm.cpp
        include/pixel.h)

target_include_directories(mikeshahchap1 PRIVATE include)