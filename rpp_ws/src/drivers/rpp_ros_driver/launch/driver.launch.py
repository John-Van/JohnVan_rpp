from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration,TextSubstitution
import os
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    config = os.path.join(
      get_package_share_directory('rpp_ros_driver'),
      'config',
      'config.yaml'
      )

    rpp_ros_driver_node= Node(
        package='rpp_ros_driver',
        executable='rpp_ros_driver_node',
        name='serial_driver',
        output='screen',
        parameters=[config]
    )
    return LaunchDescription([
        rpp_ros_driver_node
        ])
