# 2.4 用：子目录这一份，只干两件事——建一个库、说清楚它的头文件在哪儿。

add_library(tools STATIC util.cpp)

# PUBLIC 的意思是「我自己要用，用我这个库的人也得用」：
# 库自己编的时候要能找到 util.h，main.cpp 那边 #include "util.h" 也要能找到。
target_include_directories(tools PUBLIC .)
