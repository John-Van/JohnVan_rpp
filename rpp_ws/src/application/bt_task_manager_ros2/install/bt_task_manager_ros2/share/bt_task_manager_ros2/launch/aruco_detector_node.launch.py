import os
from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():

    config_file = os.path.join(get_package_share_directory("bt_task_manager_ros2"), "config", "aruco_detector_node.yaml")

    return LaunchDescription(
        [
            Node(
                package="bt_task_manager_ros2",
                namespace="",
                executable="aruco_detector_node",
                parameters=[config_file],
            )
        ]
    )
