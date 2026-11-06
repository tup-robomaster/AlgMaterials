cmake_minimum_required(VERSION 3.16)
project(robot_nodes)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(robot_interfaces REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(Threads REQUIRED)

set(NODE_TARGETS
  hello_node
  talker
  listener
  target_talker
  target_listener
  qos_talker
  qos_listener
  qos_listener_best_effort
  multi_talker
  filter_listener
  diag_talker
  add_server
  add_client
  area_server
  area_client
  parameter_node
  param_demo
  patrol_server
  patrol_client
  patrol_cancel_client
  queue_demo
  executor_probe
  group_probe
  long_task_demo
  lifecycle_talker
  lifecycle_listener
  lifecycle_manager
  yaw_to_quaternion
  quaternion_compose
  static_tf
  dynamic_tf
  tf_listener
  tf_time
  logging_demo
  target_source
  patrol_controller
)

add_executable(hello_node src/hello_node.cpp)
ament_target_dependencies(hello_node rclcpp)

add_executable(talker src/talker.cpp)
ament_target_dependencies(talker rclcpp std_msgs)

add_executable(listener src/listener.cpp)
ament_target_dependencies(listener rclcpp std_msgs)

add_executable(target_talker src/target_talker.cpp)
ament_target_dependencies(target_talker rclcpp robot_interfaces)

add_executable(target_listener src/target_listener.cpp)
ament_target_dependencies(target_listener rclcpp robot_interfaces)

add_executable(qos_talker src/qos_talker.cpp)
ament_target_dependencies(qos_talker rclcpp std_msgs)

add_executable(qos_listener src/qos_listener.cpp)
ament_target_dependencies(qos_listener rclcpp std_msgs)

add_executable(qos_listener_best_effort src/qos_listener_best_effort.cpp)
ament_target_dependencies(qos_listener_best_effort rclcpp std_msgs)

add_executable(multi_talker src/multi_talker.cpp)
ament_target_dependencies(multi_talker rclcpp std_msgs)

add_executable(filter_listener src/filter_listener.cpp)
ament_target_dependencies(filter_listener rclcpp std_msgs)

add_executable(diag_talker src/diag_talker.cpp)
ament_target_dependencies(diag_talker rclcpp std_msgs)

add_executable(add_server src/add_server.cpp)
ament_target_dependencies(add_server rclcpp example_interfaces)

add_executable(add_client src/add_client.cpp)
ament_target_dependencies(add_client rclcpp example_interfaces)

add_executable(area_server src/area_server.cpp)
ament_target_dependencies(area_server rclcpp robot_interfaces)

add_executable(area_client src/area_client.cpp)
ament_target_dependencies(area_client rclcpp robot_interfaces)

add_executable(parameter_node src/parameter_node.cpp)
ament_target_dependencies(parameter_node rclcpp rcl_interfaces)

add_executable(param_demo src/param_demo.cpp)
ament_target_dependencies(param_demo rclcpp std_msgs rcl_interfaces)

add_executable(patrol_server src/patrol_server.cpp)
ament_target_dependencies(patrol_server rclcpp rclcpp_action robot_interfaces)

add_executable(patrol_client src/patrol_client.cpp)
ament_target_dependencies(patrol_client rclcpp rclcpp_action robot_interfaces)

add_executable(patrol_cancel_client src/patrol_cancel_client.cpp)
ament_target_dependencies(patrol_cancel_client rclcpp rclcpp_action robot_interfaces)

# 第 4、5 部分的示例
add_executable(queue_demo src/queue_demo.cpp)
ament_target_dependencies(queue_demo rclcpp std_msgs)

add_executable(executor_probe src/executor_probe.cpp)
ament_target_dependencies(executor_probe rclcpp)
target_link_libraries(executor_probe Threads::Threads)

add_executable(group_probe src/group_probe.cpp)
ament_target_dependencies(group_probe rclcpp)
target_link_libraries(group_probe Threads::Threads)

add_executable(long_task_demo src/long_task_demo.cpp)
ament_target_dependencies(long_task_demo rclcpp)
target_link_libraries(long_task_demo Threads::Threads)

# 第 6、7 部分的示例
add_executable(lifecycle_talker src/6.2-lifecycle_talker.cpp)
ament_target_dependencies(lifecycle_talker rclcpp rclcpp_lifecycle std_msgs)

add_executable(lifecycle_listener src/lifecycle_listener.cpp)
ament_target_dependencies(lifecycle_listener rclcpp rclcpp_lifecycle std_msgs)

add_executable(lifecycle_manager src/6.3-lifecycle_manager.cpp)
ament_target_dependencies(lifecycle_manager rclcpp lifecycle_msgs)

add_executable(yaw_to_quaternion src/7.1-yaw_to_quaternion.cpp)
ament_target_dependencies(yaw_to_quaternion tf2 geometry_msgs)

add_executable(quaternion_compose src/7.1-quaternion_compose.cpp)
ament_target_dependencies(quaternion_compose tf2 geometry_msgs)

add_executable(static_tf src/7.2-static_tf.cpp)
ament_target_dependencies(static_tf rclcpp tf2 tf2_ros geometry_msgs)

add_executable(dynamic_tf src/7.3-dynamic_tf.cpp)
ament_target_dependencies(dynamic_tf rclcpp tf2 tf2_ros geometry_msgs)

add_executable(tf_listener src/7.4-tf_listener.cpp)
ament_target_dependencies(tf_listener rclcpp tf2 tf2_ros tf2_geometry_msgs geometry_msgs)

add_executable(tf_time src/7.5-tf_time.cpp)
ament_target_dependencies(tf_time rclcpp tf2 tf2_ros geometry_msgs)

# 第 8 部分的示例
add_executable(logging_demo src/8.1-logging.cpp)
ament_target_dependencies(logging_demo rclcpp)

# 第 9 部分的示例
add_executable(target_source src/9.2-target_source.cpp)
ament_target_dependencies(target_source rclcpp robot_interfaces)

add_executable(patrol_controller src/9.3-patrol_controller.cpp)
ament_target_dependencies(patrol_controller rclcpp rclcpp_action robot_interfaces)

add_library(talker_component SHARED src/talker_component.cpp)
ament_target_dependencies(talker_component rclcpp rclcpp_components std_msgs)
rclcpp_components_register_nodes(talker_component "robot_nodes::TalkerComponent")

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_area test/test_area.cpp src/area.cpp)
  target_include_directories(test_area PRIVATE include)

  find_package(launch_testing_ament_cmake REQUIRED)
  add_launch_test(test/test_talker.launch.py)
endif()

install(TARGETS ${NODE_TARGETS}
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS talker_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
