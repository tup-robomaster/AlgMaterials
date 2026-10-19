# 2.3 用：两个源文件 + 一个头文件目录。
#
# add_executable 里只列 .cpp（头文件不是拿来说「要编哪个程序」的），
# target_include_directories 那一行管的是「#include "stats.h" 去哪儿找」。
#
# 还有一条：target 的名字（report）只能用字母数字和 . - _ +，中文不行——
# 你 1.4 那个可执行文件就叫 report，这儿正好照搬。

cmake_minimum_required(VERSION 3.16)
project(统计表 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(report main.cpp stats.cpp)

target_include_directories(report PRIVATE include)
