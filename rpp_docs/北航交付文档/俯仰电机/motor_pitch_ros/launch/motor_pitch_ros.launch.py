from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration, TextSubstitution
import os
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory("motor_pitch_ros"), "config", "config.yaml"
    )

    motor_pitch_ros_node = Node(
        package="motor_pitch_ros",
        executable="motor_pitch_ros_node",
        name="motor_pitch_ros_node",
        output="screen",
        parameters=[config],
    )
    return LaunchDescription([motor_pitch_ros_node])
