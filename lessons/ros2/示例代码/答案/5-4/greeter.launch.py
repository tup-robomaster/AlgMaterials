# 5.4 的答案：把 hello_node 的节点名改为 greeter，
# 启动器给进程的编号仍按可执行文件名 hello_node 生成。
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='robot_nodes',
            executable='hello_node',
            name='greeter',
            output='screen',
        ),
    ])
