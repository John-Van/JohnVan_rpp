from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.conditions import LaunchConfigurationEquals
from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    calibration_type = LaunchConfiguration("calibration_type")
    tracking_base_frame = LaunchConfiguration("tracking_base_frame")
    tracking_marker_frame = LaunchConfiguration("tracking_marker_frame")
    robot_base_frame = LaunchConfiguration("robot_base_frame")
    robot_effector_frame = LaunchConfiguration("robot_effector_frame")

    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "calibration_type",
                default_value="eye_in_hand",
                choices=["eye_in_hand", "eye_on_base"],
                description="Type of the calibration",
            ),
            DeclareLaunchArgument(
                "tracking_base_frame",
                default_value="kortex_camera_color_frame",
            ),
            DeclareLaunchArgument(
                "tracking_marker_frame",
                default_value="aruco_marker_frame",
            ),
            DeclareLaunchArgument(
                "robot_base_frame",
                default_value="kortex_base_link",
            ),
            DeclareLaunchArgument(
                "robot_effector_frame",
                default_value="kortex_end_effector_link",
            ),
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource([PathJoinSubstitution([FindPackageShare("easy_handeye2"), "launch", "calibrate.launch.py"])]),
                launch_arguments={
                    "calibration_type": calibration_type,
                    "tracking_base_frame": tracking_base_frame,
                    "tracking_marker_frame": tracking_marker_frame,
                    "robot_base_frame": robot_base_frame,
                    "robot_effector_frame": robot_effector_frame,
                }.items(),
            ),
        ]
    )
