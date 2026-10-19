# 练习 2-7 用：中文那两处改过来之后的 CMakeLists.txt。
#
# 中文本身没被赶出去——它照样待在 project() 的名字里、待在字符串里，
# cmake 要的只是：**变量名和 target 名**这两个位置得是 ASCII。
cmake_minimum_required(VERSION 3.16)
project(统计表 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(DESC_TEXT "统计表（第 2 版）")
message(STATUS "这次建的是：${DESC_TEXT}")

add_executable(report main.cpp stats.cpp)
target_include_directories(report PRIVATE include)
