cmake_minimum_required(VERSION 3.16)
project(hello_mat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

find_package(OpenCV 4 REQUIRED)

add_executable(hello_mat ../0.2-hello_mat.cpp)
target_include_directories(hello_mat PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(hello_mat PRIVATE ${OpenCV_LIBS})
