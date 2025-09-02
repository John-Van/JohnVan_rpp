import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import LogInfo
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch.actions import (
    DeclareLaunchArgument,
    ExecuteProcess,
    IncludeLaunchDescription,
    Shutdown,
)
from launch.substitutions import (
    Command,
    FindExecutable,
    LaunchConfiguration,
    PathJoinSubstitution,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.substitutions import FindPackageShare

"在这里启动所有雷达，目前只考虑标准cr100故不做分类"


def create_dual_lakibeam1():
    back_lidar_2d_launch_file = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [
                PathJoinSubstitution(
                    [FindPackageShare("robot_bringup"), "launch", "lidar_2d.launch.py"]
                )
            ]
        ),
        launch_arguments={
            "namespace": "/",
            "lidar_name": "lakibeam1_back_node",
            "frame_id": "laser_B_link",
            "output_topic": "scan_multi",
            "inverted": "false",
            "hostip": "192.168.11.11",
            "sensorip": "192.168.11.20",
            "port": '"2368"',
            "angle_offset": "0",
        }.items(),
    )

    return [
        back_lidar_2d_launch_file,
    ]


def create_mid360():
    package_name = "robot_bringup"
    package_path = get_package_share_directory(package_name)
    user_config_path = os.path.join(package_path, "params", "MID360_config.json")
    lidar_3d_launch_file = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            [
                PathJoinSubstitution(
                    [FindPackageShare("robot_bringup"), "launch", "lidar_3d.launch.py"]
                )
            ]
        ),
        launch_arguments={
            "namespace": "/",
            "lidar_name": "livox_node",
            "frame_id": "lidar_link",
            "xfer_format": "1",
            "multi_topic": "0",
            "data_src": "0",
            "publish_freq": "10.0",
            "output_type": "0",
            "lvx_file_path": "/home/livox/livox_test.lvx",
            "cmdline_bd_code": "livox0000000001",
            "user_config_path": user_config_path,
        }.items(),
    )
    return [lidar_3d_launch_file]


def generate_launch_description():
    use_2d_lidar = DeclareLaunchArgument(
        "use_2d_lidar", default_value="True", description="是否使用2D雷达"
    )

    use_3d_lidar = DeclareLaunchArgument(
        "use_3d_lidar", default_value="True", description="是否使用3D雷达"
    )

    lidar_2d = create_dual_lakibeam1()
    lidar_3d = create_mid360()
    ld = LaunchDescription()
    ld.add_action(use_2d_lidar)
    ld.add_action(use_3d_lidar)
    for node in lidar_2d + lidar_3d:
        ld.add_action(node)
    return ld
    # return LaunchDescription()
