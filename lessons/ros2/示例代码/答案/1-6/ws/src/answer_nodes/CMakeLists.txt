cmake_minimum_required(VERSION 3.16)
project(answer_nodes)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(robot_interfaces REQUIRED)

add_executable(status_talker src/status_talker.cpp)
ament_target_dependencies(status_talker rclcpp robot_interfaces)

install(TARGETS status_talker
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
