from launch import LaunchDescription
from launch_ros.actions import Node
def generate_launch_description():

    return LaunchDescription(
        [
            Node(
                package="realsense2_camera",
                executable="realsense2_camera_node",
                name="camera",
                namespace="head",
                parameters=[
                    {
                        "serial_no": "318122302473",
                        "camera_name": "head_camera",
                        "publish_tf":False,
                        "enable_color": True,
                        "enable_depth": True,
                        "pointcloud.enable": True,
                        "align_depth.enable": True,
                        "rgb_camera.color_profile": "640,480,30",
                        "depth_module.depth_profile": "640,480,30",
                    }
                ],
                output="screen",
            ),
            Node(
                package="realsense2_camera",
                executable="realsense2_camera_node",
                name="camera",
                namespace="left",
                parameters=[
                    {
                        "serial_no": "139522074289",
                        "camera_name": "left_camera",
                        "publish_tf":False,
                        "enable_color": True,
                        "enable_depth": True,
                        "enable_infra":False,
                        "enable_infra1":False,
                        "enable_infra2":False,
                        "pointcloud.enable": True,
                        "align_depth.enable": True,
                        "rgb_camera.color_profile": "640,480,30",
                        "depth_module.depth_profile": "640,480,30",
                    }
                ],
                output="screen",
            ),
            Node(
                package="realsense2_camera",
                executable="realsense2_camera_node",
                name="camera",
                namespace="right",
                parameters=[
                    {
                        "serial_no": "339222071403",
                        "camera_name": "right_camera",
                        "publish_tf":False,
                        "enable_color": True,
                        "enable_depth": True,
                        "enable_infra":False,
                        "enable_infra1":False,
                        "enable_infra2":False,
                        "pointcloud.enable": True,
                        "align_depth.enable": True,
                        "rgb_camera.color_profile": "640,480,30",
                        "depth_module.depth_profile": "640,480,30",
                    }
                ],
                output="screen",
            ),
        ]
    )
