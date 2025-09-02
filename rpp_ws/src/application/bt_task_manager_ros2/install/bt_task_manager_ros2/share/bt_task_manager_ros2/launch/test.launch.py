import os

from launch import LaunchDescription

from launch.substitutions import LaunchConfiguration, TextSubstitution
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.substitutions import PathJoinSubstitution

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
import os

from ament_index_python.packages import get_package_share_directory


def generate_launch_description():

    # config_file = os.path.join(get_package_share_directory("bt_task_manager_ros2"), "config", "test.yaml")

    config_file = LaunchConfiguration("config_file", default=os.path.join(get_package_share_directory("bt_task_manager_ros2"), "config", "test.yaml"))
    return LaunchDescription(
        [
            DeclareLaunchArgument("config_file", default_value=config_file, description="Specifying parameter direction"),
            Node(
                package="bt_task_manager_ros2",
                executable="test",
                parameters=[config_file],
            ),
            # Node(package="tf2_ros", executable="static_transform_publisher", arguments=["0.4", "0", "0.2", "0", "0", "0", "link_base", "goal"]),
        ]
    )


if __name__ == "__main__":
    a = generate_launch_description()
    print(a)
