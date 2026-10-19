cmake_minimum_required(VERSION 3.16)
project(分段统计 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(demo main.cpp stats.cpp)
