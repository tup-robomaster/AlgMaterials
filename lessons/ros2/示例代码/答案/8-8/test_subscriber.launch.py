import time
import unittest

import launch
import launch_ros.actions
import launch_testing
import launch_testing.actions
import launch_testing.asserts
import rclpy
from std_msgs.msg import String


def generate_test_description():
    talker = launch_ros.actions.Node(
        package="robot_nodes",
        executable="talker",
        arguments=["2"],
        parameters=[{"wait_for_subscriber": True}],
    )
    return (
        launch.LaunchDescription(
            [
                talker,
                launch_testing.actions.ReadyToTest(),
            ]
        ),
        {"talker": talker},
    )


class TestSubscriber(unittest.TestCase):
    def test_first_message(self):
        rclpy.init()
        node = rclpy.create_node("test_subscriber")
        messages = []
        node.create_subscription(
            String, "/chatter", lambda msg: messages.append(msg.data), 10
        )
        deadline = time.time() + 5.0
        while time.time() < deadline and not messages:
            rclpy.spin_once(node, timeout_sec=0.1)
        node.destroy_node()
        rclpy.shutdown()
        self.assertTrue(messages, "没有收到 /chatter 消息")
        self.assertEqual(messages[0], "hello 0")


@launch_testing.post_shutdown_test()
class TestExit(unittest.TestCase):
    def test_exit_code(self, proc_info):
        launch_testing.asserts.assertExitCodes(proc_info, allowable_exit_codes=[0])
