from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    start_server = LaunchConfiguration("start_server")
    server_namespace = LaunchConfiguration("server_namespace")
    steps = LaunchConfiguration("steps_per_target")
    delay = LaunchConfiguration("start_delay_ms")

    static_tf = Node(
        package="robot_nodes",
        executable="static_tf",
        parameters=[
            {
                "parent_frame": "map",
                "child_frame": "base_link",
                "x": 0.0,
                "y": 0.0,
                "z": 0.0,
                "yaw_deg": 0.0,
            }
        ],
    )

    patrol_server = Node(
        package="robot_nodes",
        executable="patrol_server",
        namespace=server_namespace,
        condition=IfCondition(start_server),
    )

    controller = Node(
        package="robot_nodes",
        executable="patrol_controller",
        parameters=[
            {"initial_mode": "patrol"},
            {"steps_per_target": ParameterValue(steps, value_type=int)},
            {"finish_when_empty": True},
        ],
    )

    target_source = Node(
        package="robot_nodes",
        executable="target_source",
        parameters=[
            {"start_delay_ms": ParameterValue(delay, value_type=int)},
            {"max_distance": 0.0},
            {"wait_for_subscriber": True},
        ],
    )

    return LaunchDescription(
        [
            DeclareLaunchArgument("start_server", default_value="true"),
            DeclareLaunchArgument("server_namespace", default_value=""),
            DeclareLaunchArgument("steps_per_target", default_value="1"),
            DeclareLaunchArgument("start_delay_ms", default_value="1500"),
            static_tf,
            patrol_server,
            controller,
            target_source,
        ]
    )
