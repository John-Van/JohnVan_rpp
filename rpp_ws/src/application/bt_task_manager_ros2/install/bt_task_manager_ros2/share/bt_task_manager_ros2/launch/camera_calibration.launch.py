import os
import yaml

from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    camera_calibration_file = os.path.join(get_package_share_directory("bt_task_manager_ros2"), "config", "camera_calibration.yaml")

    with open(camera_calibration_file, "r") as file:
        config = yaml.safe_load(file)

    translation = config["translation"]
    rpy = config["euler"]
    print(
        [
            str(translation["x"]),
            str(translation["y"]),
            str(translation["z"]),
            str(rpy["roll"]),
            str(rpy["pitch"]),
            str(rpy["yaw"]),
            "kortex_end_effector_link",
            "kortex_camera_color_frame",
        ],
    )
    return LaunchDescription(
        [
            Node(
                package="tf2_ros",
                executable="static_transform_publisher",
                arguments=[
                    str(translation["x"]),
                    str(translation["y"]),
                    str(translation["z"]),
                    str(rpy["roll"]),
                    str(rpy["pitch"]),
                    str(rpy["yaw"]),
                    "kortex_end_effector_link",
                    "kortex_camera_color_frame",
                ],
            ),
        ]
    )
