# 练习 2-9 用：照那份真实工程的骨架写的一份小的。
#
# 三段：开头几行说清「要什么」；中间把源文件列成一张清单；最后才是
# 「编哪个、去哪儿找头文件」。清单那个写法（set 一个变量、下面用 ${} 取它）
# 是照着那份真实工程抄的——它三十来个 .cpp/.cu 就是这么一行一个列下来的。

cmake_minimum_required(VERSION 3.16)

project(统计表 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall)

# 源文件清单。一个 .cpp 一行，路径跟着文件走。
set(stats_source
    main.cpp
    Modules/统计/src/stats.cpp)

add_executable(stats ${stats_source})

target_include_directories(stats PRIVATE Modules/统计/include)
