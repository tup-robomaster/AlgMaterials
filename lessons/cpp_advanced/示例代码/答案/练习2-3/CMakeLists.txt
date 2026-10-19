# 2.2 用：最小的一份 CMakeLists.txt，四行。
# 每一行在说什么，看 2.2 正文；这四行和 0.3 里见过的那份一模一样。

cmake_minimum_required(VERSION 3.16)
project(demo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(demo main.cpp)
