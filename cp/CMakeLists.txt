cmake_minimum_required(VERSION 3.23)
project(cp)

set(CMAKE_CXX_STANDARD 20)

add_executable(cp main.cpp)
target_include_directories(cp PUBLIC ../utils)