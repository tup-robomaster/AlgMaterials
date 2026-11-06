cmake_minimum_required(VERSION 3.16)
project(robot_answers)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(robot_interfaces REQUIRED)
find_package(answer_interfaces REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

set(ANSWER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../")

add_executable(a0_3 "${ANSWER_DIR}/0-3/hello_1s.cpp")
ament_target_dependencies(a0_3 rclcpp)

add_executable(a0_4 "${ANSWER_DIR}/0-4/tick_node.cpp")
ament_target_dependencies(a0_4 rclcpp)

add_executable(a0_8 "${ANSWER_DIR}/0-8/info_node.cpp")
ament_target_dependencies(a0_8 rclcpp)

add_executable(a1_1 "${ANSWER_DIR}/1-1/counter_talker.cpp")
ament_target_dependencies(a1_1 rclcpp std_msgs)

add_executable(a1_2 "${ANSWER_DIR}/1-2/prefix_talker.cpp")
ament_target_dependencies(a1_2 rclcpp std_msgs)

add_executable(a1_3 "${ANSWER_DIR}/1-3/even_listener.cpp")
ament_target_dependencies(a1_3 rclcpp std_msgs)

add_executable(a1_3_numbers "${ANSWER_DIR}/1-3/number_talker.cpp")
ament_target_dependencies(a1_3_numbers rclcpp std_msgs)

add_executable(a1_4 "${ANSWER_DIR}/1-4/length_listener.cpp")
ament_target_dependencies(a1_4 rclcpp std_msgs)

add_executable(a1_4_words "${ANSWER_DIR}/1-4/word_talker.cpp")
ament_target_dependencies(a1_4_words rclcpp std_msgs)

add_executable(a1_7_talker "${ANSWER_DIR}/1-7/late_talker.cpp")
ament_target_dependencies(a1_7_talker rclcpp std_msgs)

add_executable(a1_7_listener "${ANSWER_DIR}/1-7/late_listener.cpp")
ament_target_dependencies(a1_7_listener rclcpp std_msgs)

add_executable(a1_8_talker "${ANSWER_DIR}/1-8/sensor_talker.cpp")
ament_target_dependencies(a1_8_talker rclcpp std_msgs)

add_executable(a1_8_listener "${ANSWER_DIR}/1-8/sensor_listener.cpp")
ament_target_dependencies(a1_8_listener rclcpp std_msgs)

add_executable(a1_10 "${ANSWER_DIR}/1-10/prefix_filter.cpp")
ament_target_dependencies(a1_10 rclcpp std_msgs)

add_executable(a1_11 "${ANSWER_DIR}/1-11/probe_talker.cpp")
ament_target_dependencies(a1_11 rclcpp std_msgs)

add_executable(a1_10_words "${ANSWER_DIR}/1-10/prefix_words.cpp")
ament_target_dependencies(a1_10_words rclcpp std_msgs)

add_executable(a2_1 "${ANSWER_DIR}/ws/src/robot_answers/src/a2_1.cpp")
ament_target_dependencies(a2_1 rclcpp example_interfaces)

add_executable(a2_2 "${ANSWER_DIR}/ws/src/robot_answers/src/a2_2.cpp")
ament_target_dependencies(a2_2 rclcpp robot_interfaces)

add_executable(a2_3 "${ANSWER_DIR}/ws/src/robot_answers/src/a2_3.cpp")
ament_target_dependencies(a2_3 rclcpp example_interfaces)

add_executable(a2_4 "${ANSWER_DIR}/ws/src/robot_answers/src/a2_4.cpp")
ament_target_dependencies(a2_4 rclcpp example_interfaces)

add_executable(a2_6 "${ANSWER_DIR}/ws/src/robot_answers/src/a2_6.cpp")
ament_target_dependencies(a2_6 rclcpp robot_interfaces)

add_executable(a2_7 "${ANSWER_DIR}/ws/src/robot_answers/src/a2_7.cpp")
ament_target_dependencies(a2_7 rclcpp)

add_executable(a2_8 "${ANSWER_DIR}/ws/src/robot_answers/src/a2_8.cpp")
ament_target_dependencies(a2_8 rclcpp rcl_interfaces)

add_executable(a3_3 "${ANSWER_DIR}/ws/src/robot_answers/src/a3_3.cpp")
ament_target_dependencies(a3_3 rclcpp rclcpp_action robot_interfaces)

add_executable(a3_4 "${ANSWER_DIR}/ws/src/robot_answers/src/a3_4.cpp")
ament_target_dependencies(a3_4 rclcpp rclcpp_action robot_interfaces)

add_executable(a3_4_client "${ANSWER_DIR}/ws/src/robot_answers/src/a3_4_client.cpp")
ament_target_dependencies(a3_4_client rclcpp rclcpp_action robot_interfaces)

add_executable(a3_5 "${ANSWER_DIR}/ws/src/robot_answers/src/a3_5.cpp")
ament_target_dependencies(a3_5 rclcpp rclcpp_action robot_interfaces)

add_executable(a3_6 "${ANSWER_DIR}/ws/src/robot_answers/src/a3_6.cpp")
ament_target_dependencies(a3_6 rclcpp rclcpp_action robot_interfaces)

add_executable(a3_7 "${ANSWER_DIR}/ws/src/robot_answers/src/a3_7.cpp")
ament_target_dependencies(a3_7 rclcpp rclcpp_action robot_interfaces)

add_executable(a3_8 "${ANSWER_DIR}/ws/src/robot_answers/src/a3_8.cpp")
ament_target_dependencies(a3_8 rclcpp rclcpp_action robot_interfaces)

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
  a0_3 a0_4 a0_8
  a1_1 a1_2 a1_3 a1_3_numbers a1_4 a1_4_words
  a1_7_talker a1_7_listener a1_8_talker a1_8_listener
  a1_10 a1_10_words a1_11
  a2_1 a2_2 a2_3 a2_4 a2_6 a2_7 a2_8
  a3_3 a3_4 a3_4_client a3_5 a3_6 a3_7 a3_8
  a10_3_publisher a10_3_server a10_4_server a10_4_client
  a10_6_broadcaster a10_6_listener
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
  OPTIONAL
)

ament_package()
