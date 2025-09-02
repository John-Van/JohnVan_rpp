from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node


def generate_launch_description():

    marker_size = LaunchConfiguration("marker_size")
    marker_id = LaunchConfiguration("marker_id")
    reference_frame = LaunchConfiguration("reference_frame")
    camera = LaunchConfiguration("camera")
    camera_frame = LaunchConfiguration("camera_frame")
    marker_frame = LaunchConfiguration("marker_frame")
    corner_refinement = LaunchConfiguration("corner_refinement")
    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "marker_id",
                default_value="6",
                description="Marker ID. ",
            ),
            DeclareLaunchArgument(
                "marker_size",
                default_value="0.039",
                description="Marker size in m. ",
            ),
            DeclareLaunchArgument(
                "camera",
                default_value="kortex_camera",
                description="Which the camera will be used. ",
            ),
            DeclareLaunchArgument(
                "camera_frame",
                default_value="kortex_camera_color_frame",
                description="Frame in which the camera will be refered. ",
            ),
            DeclareLaunchArgument(
                "marker_frame",
                default_value="aruco_marker_frame",
                description="Frame in which the marker pose will be refered. ",
            ),
            DeclareLaunchArgument(
                "reference_frame",
                default_value="kortex_camera_color_frame",
                description="Reference frame. " "Leave it empty and the pose will be published wrt param parent_name. ",
            ),
            DeclareLaunchArgument(
                "corner_refinement", default_value="SUBPIX", description="Corner Refinement. ", choices=["NONE", "HARRIS", "LINES", "SUBPIX"]
            ),
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource([PathJoinSubstitution([FindPackageShare("aruco_ros"), "launch", "single.launch.py"])]),
                launch_arguments={
                    "marker_size": marker_size,
                    "marker_id": marker_id,
                    "reference_frame": reference_frame,
                    "camera": camera,
                    "camera_frame": camera_frame,
                    "marker_frame": marker_frame,
                    "corner_refinement": corner_refinement,
                }.items(),
            ),
        ]
    )
