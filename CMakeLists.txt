cmake_minimum_required(VERSION 2.8.4)
project(cpp_blend)

include_directories(src/main src/test)
set(SOURCE_FILES src/test/main.cpp)
add_executable(cpp_blend ${SOURCE_FILES})

ADD_DEFINITIONS(
    -std=c++1y
)