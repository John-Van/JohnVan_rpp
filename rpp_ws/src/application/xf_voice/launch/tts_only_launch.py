from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    config_path = os.path.join(
        get_package_share_directory('xf_voice'),
        'config',
        'tts_params.yaml'
    )

    return LaunchDescription([
        Node(
            package='xf_voice',
            executable='tts_subscribe',
            name='tts_node',
            output='screen',
            parameters=[config_path]
        )
    ])
