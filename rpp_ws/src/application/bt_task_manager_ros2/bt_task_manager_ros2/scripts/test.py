#!/usr/bin/env python3

import rclpy
import rclpy.node

from bt_task_manager_ros2.moveit_arm import *
from bt_task_manager_ros2.get_grasp_pose import *


class Test(rclpy.node.Node):
    def __init__(self):
        super().__init__("test", allow_undeclared_parameters=False, automatically_declare_parameters_from_overrides=True)


        # 声明参数
        self.declare_parameter("robot_info.name")
        self.declare_parameter("robot_info.speed")
        self.declare_parameter("robot_info.sensors.camera")
        self.declare_parameter("robot_info.sensors.lidar")

        # 获取参数并手动组装为字典
        robot_name = self.get_parameter("robot_info.name").value
        robot_speed = self.get_parameter("robot_info.speed").value
        camera_status = self.get_parameter("robot_info.sensors.camera").value
        lidar_status = self.get_parameter("robot_info.sensors.lidar").value

        # 将这些参数重新组装成一个字典
        robot_info = {"name": robot_name, "speed": robot_speed, "sensors": {"camera": camera_status, "lidar": lidar_status}}

        self.get_logger().info(f"Robot Info: {robot_info}")


def main(args=None):
    rclpy.init(args=args)
    node = Test()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
