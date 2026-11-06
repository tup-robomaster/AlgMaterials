cmake_minimum_required(VERSION 3.16)
project(part10_answers)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(answer_interfaces REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

set(ANSWER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../")

add_executable(a10_3_publisher "${ANSWER_DIR}/10-3/reading_publisher.cpp")
ament_target_dependencies(a10_3_publisher rclcpp answer_interfaces)

add_executable(a10_3_server "${ANSWER_DIR}/10-3/scale_server.cpp")
ament_target_dependencies(a10_3_server rclcpp answer_interfaces)

add_executable(a10_4_server "${ANSWER_DIR}/10-4/countdown_server.cpp")
ament_target_dependencies(a10_4_server rclcpp rclcpp_action answer_interfaces)

add_executable(a10_4_client "${ANSWER_DIR}/10-4/countdown_client.cpp")
ament_target_dependencies(a10_4_client rclcpp rclcpp_action answer_interfaces)

add_executable(a10_6_broadcaster "${ANSWER_DIR}/10-6/tf_broadcaster.cpp")
ament_target_dependencies(a10_6_broadcaster rclcpp tf2_ros geometry_msgs)

add_executable(a10_6_listener "${ANSWER_DIR}/10-6/tf_point_listener.cpp")
ament_target_dependencies(a10_6_listener rclcpp tf2_ros tf2_geometry_msgs geometry_msgs)

install(TARGETS
  a10_3_publisher a10_3_server a10_4_server a10_4_client
  a10_6_broadcaster a10_6_listener
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
