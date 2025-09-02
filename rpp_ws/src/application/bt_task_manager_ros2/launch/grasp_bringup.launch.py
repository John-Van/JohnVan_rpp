from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch_ros.substitutions import FindPackageShare


def generate_launch_description():

    return LaunchDescription(
        [
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    [
                        PathJoinSubstitution(
                            [
                                FindPackageShare("bt_task_manager_ros2"),
                                "launch",
                                "arm.launch.py",
                            ]
                        )
                    ]
                ),
            ),
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    [
                        PathJoinSubstitution(
                            [
                                FindPackageShare("bt_task_manager_ros2"),
                                "launch",
                                "camera.launch.py",
                            ]
                        )
                    ]
                ),
            ),
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    [
                        PathJoinSubstitution(
                            [
                                FindPackageShare("bt_task_manager_ros2"),
                                "launch",
                                "camera_calibration.launch.py",
                            ]
                        )
                    ]
                ),
            ),
        ]
    )
