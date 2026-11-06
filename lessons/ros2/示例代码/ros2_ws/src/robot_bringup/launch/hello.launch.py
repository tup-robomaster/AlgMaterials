# 5.2 的 Python 启动文件：启动一个 hello_node，节点打印三行后自行退出。
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package='robot_nodes',
            executable='hello_node',
            name='hello_node',
            output='screen',
        ),
    ])
