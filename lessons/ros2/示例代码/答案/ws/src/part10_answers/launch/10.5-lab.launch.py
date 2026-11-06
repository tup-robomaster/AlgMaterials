from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    talker = Node(
        package="robot_nodes",
        executable="talker",
        namespace="lab",
        remappings=[("chatter", "lab_chatter")],
        parameters=[{"wait_for_subscriber": True}],
    )

    listener = Node(
        package="robot_nodes",
        executable="listener",
        namespace="lab",
        remappings=[("chatter", "lab_chatter")],
    )

    parameter_node = Node(
        package="robot_nodes",
        executable="parameter_node",
        namespace="lab",
        parameters=[{"rate_ms": 250, "message": "lab"}],
    )

    return LaunchDescription([talker, listener, parameter_node])
