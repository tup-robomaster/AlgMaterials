cmake_minimum_required(VERSION 3.16)
project(answer_nodes)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(robot_interfaces REQUIRED)

add_executable(circle_server src/circle_server.cpp)
ament_target_dependencies(circle_server rclcpp robot_interfaces)

add_executable(circle_client src/circle_client.cpp)
ament_target_dependencies(circle_client rclcpp robot_interfaces)

install(TARGETS circle_server circle_client
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
