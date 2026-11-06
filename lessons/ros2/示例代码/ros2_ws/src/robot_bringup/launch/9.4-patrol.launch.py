from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, Shutdown
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    steps = LaunchConfiguration("steps_per_target")
    delay = LaunchConfiguration("start_delay_ms")
    max_distance = LaunchConfiguration("max_distance")
    initial_mode = LaunchConfiguration("initial_mode")

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
    )

    controller = Node(
        package="robot_nodes",
        executable="patrol_controller",
        parameters=[
            {"initial_mode": initial_mode},
            {"steps_per_target": ParameterValue(steps, value_type=int)},
            {"finish_when_empty": True},
        ],
        on_exit=Shutdown(),
    )

    target_source = Node(
        package="robot_nodes",
        executable="target_source",
        parameters=[
            {"start_delay_ms": ParameterValue(delay, value_type=int)},
            {"max_distance": ParameterValue(max_distance, value_type=float)},
            {"wait_for_subscriber": True},
        ],
    )

    return LaunchDescription(
        [
            DeclareLaunchArgument("steps_per_target", default_value="2"),
            DeclareLaunchArgument("start_delay_ms", default_value="1500"),
            DeclareLaunchArgument("max_distance", default_value="0.0"),
            DeclareLaunchArgument("initial_mode", default_value="patrol"),
            static_tf,
            patrol_server,
            controller,
            target_source,
        ]
    )
