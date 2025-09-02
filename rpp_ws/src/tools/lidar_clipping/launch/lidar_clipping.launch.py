# launch lidar_clipping node
from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    config = os.path.join(
      get_package_share_directory('lidar_clipping'),
      'config',
      'config.yaml'
      )

    robuster_driver= Node(
        package='lidar_clipping',
        executable='lidar_clipping',
        output='screen',
        parameters=[config]
    )
    return LaunchDescription([
        robuster_driver
        ])
