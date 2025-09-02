from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration, TextSubstitution
import os
from ament_index_python.packages import get_package_share_directory


def create_rpp_parameter_name():
    """
    声明所有参数并返回参数声明的列表。
    """
    publish_odom = DeclareLaunchArgument(
        "publish_odom", default_value="true", description=""
    )
    publish_odom_tf = DeclareLaunchArgument(
        "publish_odom_tf", default_value="true", description=""
    )
    return [publish_odom, publish_odom_tf]


def generate_launch_description():
    config = os.path.join(
        get_package_share_directory("robot_bringup"), "params", "rpp.yaml"
    )
    parameters_ = create_rpp_parameter_name()
    publish_odom = LaunchConfiguration("publish_odom")
    publish_odom_tf = LaunchConfiguration("publish_odom_tf")

    # 通过重写yaml的方式提供两种模式

    rpp_ros_driver = Node(
        package="rpp_ros_driver",
        executable="rpp_ros_driver_node",
        output="screen",
        parameters=[
            config,
            {
                "publish_odom": publish_odom,
                "publish_odom_tf": publish_odom_tf,
            },
        ],
    )
    return LaunchDescription(parameters_ + [rpp_ros_driver])
