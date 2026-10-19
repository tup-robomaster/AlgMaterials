# 练习 2-8 用：一张「命令桌」——常用的那十来条摆开，一条一条真跑一遍。
# 正文里按编号讲；这儿的注释只留一句话，省得文件太长。

cmake_minimum_required(VERSION 3.16)

# ① project：工程名、版本号、用什么语言。版本号后面的 configure_file 要用
project(命令桌 VERSION 1.2.0 LANGUAGES CXX)

# ② set：定义一个名字并保存值，使用时写 ${名字}
set(DESC_TEXT "命令桌")

# ③ message：打印一句。STATUS 是「正常信息」那一档
message(STATUS "开始配置：${DESC_TEXT}")

# ④ option + if：一个开关，第二个位置是默认值
option(EXTRA_LINE "打开的话程序多打一行" OFF)
if(EXTRA_LINE)
  message(STATUS "要多打一行：开")
else()
  message(STATUS "要多打一行：关")
endif()

# ⑤ find_package：在本机查找「多线程库的位置」
find_package(Threads REQUIRED)
message(STATUS "线程找着了：${Threads_FOUND}")
message(STATUS "还要额外链的线程库：${CMAKE_THREAD_LIBS_INIT}")

# ⑥ add_subdirectory：把子目录那份 CMakeLists.txt 也拉进来一起配
add_subdirectory(工具库)

# ⑦ configure_file：拿一个模板生成一个头文件，@变量@ 会被换掉
configure_file(include/version.h.in ${CMAKE_CURRENT_BINARY_DIR}/生成/version.h @ONLY)

# ⑧ add_compile_options：给这个目录底下所有编译命令都加一段参数
add_compile_options(-Wall)

# ⑨ add_executable 加两个 target_*：这才是「要编什么、去哪儿找头文件、链谁」
add_executable(desk main.cpp)
target_include_directories(desk PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/生成)
target_link_libraries(desk PRIVATE tools Threads::Threads)

# ⑩ install：说明「安装到其他目录」时复制哪些文件
install(TARGETS desk DESTINATION bin)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/生成/version.h DESTINATION include)
