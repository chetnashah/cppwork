cmake_minimum_required(VERSION 3.0.0)
project(learningcppmemerrors VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_BUILD_TYPE Debug)

include(CTest)
enable_testing()

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(learningcppmemerrors use_after_free.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
