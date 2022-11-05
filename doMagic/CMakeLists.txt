cmake_minimum_required(VERSION 3.23)
project(doMagic)

set(CMAKE_CXX_STANDARD 14)

add_executable(doMagic main.cpp doMagic.cpp)
target_compile_options(doMagic PUBLIC -g3)
