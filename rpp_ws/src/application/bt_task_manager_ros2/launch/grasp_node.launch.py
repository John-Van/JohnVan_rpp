import os

from launch import LaunchDescription

from launch_ros.actions import Node
import os

from ament_index_python.packages import get_package_share_directory

params = {
    "moveit_arm_config": "gen3_config",
    "moveit_gripper_config": "gen3_robotiq_2f_85_config",
    "base_frame": "kortex_base_link",
    "target_frame": "aruco_marker_frame",
    "tcp_offset_x": 0.01,
    "tcp_offset_y": 0.0,
    "tcp_offset_z": -0.11,
    "arm_feed_depth": 0.1,
    "grasp_angles_x": 0.0,
    "grasp_angles_y": 0.0,
    "grasp_angles_z": 0.0,
    "use_target_orient": False,
    "grasp_angles": [90.0, 0.0, 90.0],
}


def generate_launch_description():

    config_file = os.path.join(get_package_share_directory("bt_task_manager_ros2"), "config", "grasp_node.yaml")

    return LaunchDescription(
        [
            Node(
                package="bt_task_manager_ros2",
                executable="grasp_node",
                parameters=[config_file],
            ),
        ]
    )
