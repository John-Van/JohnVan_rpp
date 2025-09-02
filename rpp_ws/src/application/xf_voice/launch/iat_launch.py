from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    return LaunchDescription([
        # 参数声明
        DeclareLaunchArgument('package_path', default_value='/home/'),
        DeclareLaunchArgument('iat_appid', default_value='9a7a7ebd'),
        DeclareLaunchArgument('iat_maxTime', default_value='15'),

        # 语音听写节点（iat_publish）
        Node(
            package='xf_voice',
            executable='iat_publish',
            name='iat_node',
            output='screen',
            parameters=[{
                'iat_appid': LaunchConfiguration('iat_appid'),
                'maxTime': LaunchConfiguration('iat_maxTime'),
                'package_path': LaunchConfiguration('package_path')
            }]
        )
    ])
