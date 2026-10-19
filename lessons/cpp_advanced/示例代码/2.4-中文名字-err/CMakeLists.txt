cmake_minimum_required(VERSION 3.16)
project(统计表 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(说明文字 "统计表（第 2 版）")
message(STATUS "这次建的是：${说明文字}")

add_executable(报表 main.cpp stats.cpp)
target_include_directories(report PRIVATE include)
