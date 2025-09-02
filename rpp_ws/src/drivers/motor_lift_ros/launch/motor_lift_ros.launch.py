from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration, TextSubstitution
import os
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory("motor_lift_ros"), "config", "config.yaml"
    )

    motor_lift_ros_node = Node(
        package="motor_lift_ros",
        executable="motor_lift_ros_node",
        name="motor_lift_ros_node",
        output="screen",
        parameters=[config],
    )

    return LaunchDescription([motor_lift_ros_node])
