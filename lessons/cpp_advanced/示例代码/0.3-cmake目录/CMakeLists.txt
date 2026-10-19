# 0.3 用：四行，够把一个单文件程序编出来。
# 四行各是什么意思，第 2 部分一句一句讲。

cmake_minimum_required(VERSION 3.16)
project(demo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(demo main.cpp)
