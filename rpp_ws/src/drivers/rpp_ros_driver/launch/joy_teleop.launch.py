from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    use_sim_time = LaunchConfiguration("use_sim_time", default="false")
    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "use_sim_time",
                default_value="false",
                description="Use simulation (Gazebo) clock if true",
            ),
            # 启动 joy_node，用于读取控制器输入
            Node(
                package="joy",
                executable="joy_node",
                name="joy_node",
                output="screen",
                parameters=[{"device_id": 0, "use_sim_time": use_sim_time}],  
            ),
            # 启动 teleop_twist_joy，将控制器输入转换为 cmd_vel
            Node(
                package="teleop_twist_joy",
                executable="teleop_node",
                name="teleop_twist_joy",
                output="screen",
                parameters=[
                    {
                        "enable_button": 4,  # 启用控制的按钮编号
                        "axis_linear.x": 1,  # 前进/后退轴（左摇杆上下）
                        "axis_linear.y": 0,  # 左/右轴（左摇杆左右）
                        "scale_linear.x": 0.2,  # 线速度比例
                        "scale_linear.y": 0.2,  # 线速度比例
                        "axis_angular.yaw": 2,  # 旋转轴（右摇杆左右）
                        "scale_angular": 1.0,  # 角速度比例
                        "use_sim_time": use_sim_time,
                    }
                ],
            ),
        ]
    )
