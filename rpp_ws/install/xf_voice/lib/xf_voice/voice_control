#!/usr/bin/env python3
import os
import re
import rclpy
from rclpy.node import Node
from std_msgs.msg import Int8, String, Empty, UInt32
from xf_voice_msgs.srv import Xf
from match_demo import CommandMatcher
from robot_state import RobotState
from geometry_msgs.msg import PoseStamped
from tf_transformations import quaternion_from_euler
from geometry_msgs.msg import TransformStamped
from tf2_ros import Buffer, TransformListener
import numpy as np
import math
import time
import json
from geometry_msgs.msg import Twist
from nav2_simple_commander.robot_navigator import BasicNavigator
from tf_transformations import euler_from_quaternion
from nav2_msgs.action import NavigateToPose


class VoiceControlNode(Node):
    def __init__(self):
        super().__init__("voice_control_node")

        # Declare and get parameters
        self.declare_parameter("package_path", "/home/robuster/rpp_ws/src/xf_voice_")
        self.declare_parameter("iat_wait_max_time", 20)
        self.declare_parameter("iat_wait_execution_times", 3)
        self.declare_parameter("mineral_water_pos_id", 2)
        self.declare_parameter("juice_pos_id", 2)
        self.get_logger().warn("尚未获取 base_angle，无法转向")
        self.package_path = self.get_parameter("package_path").value
        self.iat_wait_max_time = self.get_parameter("iat_wait_max_time").value
        self.iat_wait_execution_times = self.get_parameter(
            "iat_wait_execution_times"
        ).value
        self.mineral_water_pos_id = self.get_parameter("mineral_water_pos_id").value
        self.juice_pos_id = self.get_parameter("juice_pos_id").value

        self.robot_new_sate = RobotState.IDLE
        self.robot_current_sate = RobotState.IDLE
        self.commandMatcher = None
        self.base_angle = None
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)
        self.cli = self.create_client(Xf, "/tts_service")
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info("等待 TTS 服务启动中...")
        self.tf_transform = None
        self.init_commandMatcher()
        self.init_publishers()
        self.init_subscriptions()
        self.cold_time = self.get_clock().now().seconds_nanoseconds()[0]
        self.execution_times = 0
        ############------------------------------------------------
        self.pose_list = []  # 位置信息
        self.pose_next = 0
        # 加载配置文件 ......省略
        self.load_pose_list_from_json(
            "/home/rpp//rpp_data/config/voice/voice_pose.json"
        )

        ###########-------------------

    def clear_pose_list(self):
        self.pose_list = []  #
        self.pose_next = 0
        self.speak("已清除所有保存的点位")

    def save_cure_pose_list(self):
        pose = self.get_current_pose_from_tf(
            self.tf_buffer, self, target_frame="map", source_frame="base_link"
        )
        if pose:
            self.pose_list.append(pose)
            self.speak(f"当前位置已保存为第 {len(self.pose_list)} 个点")
        else:
            self.speak("无法获取当前位姿，保存失败")

    def move_pose(self):
        if len(self.pose_list) == 0:
            self.speak("没有可前往的点位")
            return
        current_pose = self.get_current_pose_from_tf(
            self.tf_buffer, self, target_frame="map", source_frame="base_link"
        )
        if not current_pose:
            self.speak("无法获取当前位置，导航失败")
            return
        target_pose = self.pose_list[self.pose_next]
        # 判断当前是否已到目标附近
        dx = target_pose.pose.position.x - current_pose.pose.position.x
        dy = target_pose.pose.position.y - current_pose.pose.position.y
        distance = math.hypot(dx, dy)
        if distance < 0.1:
            self.speak("当前已在目标点附近")
        else:
            self.goal_pose_pub.publish(target_pose)
            self.speak(f"前往第 {self.pose_next + 1} 个点位")

    def move_pose_addition(self):
        if len(self.pose_list) == 0:
            self.pose_next = 0
            self.speak("没有保存的点位，无法前进")
        else:
            self.pose_next = (self.pose_next + 1) % len(self.pose_list)
            target = self.pose_list[self.pose_next]
            self.speak(
                f"切换到下一个点位：第 {self.pose_next + 1} 个，坐标 x={target.pose.position.x:.2f}, y={target.pose.position.y:.2f}"
            )

    def move_pose_subtraction(self):
        if len(self.pose_list) == 0:
            self.pose_next = 0
            self.speak("没有保存的点位，无法返回")
        else:
            self.pose_next = (self.pose_next - 1 + len(self.pose_list)) % len(
                self.pose_list
            )
            target = self.pose_list[self.pose_next]
            self.speak(
                f"返回到上一个点位：第 {self.pose_next + 1} 个，坐标 x={target.pose.position.x:.2f}, y={target.pose.position.y:.2f}"
            )

    def save_pose_list_to_json(self, filename="pose_list.json"):
        data = []
        for pose in self.pose_list:
            data.append(
                {
                    "frame_id": pose.header.frame_id,
                    "position": {
                        "x": pose.pose.position.x,
                        "y": pose.pose.position.y,
                        "z": pose.pose.position.z,
                    },
                    "orientation": {
                        "x": pose.pose.orientation.x,
                        "y": pose.pose.orientation.y,
                        "z": pose.pose.orientation.z,
                        "w": pose.pose.orientation.w,
                    },
                }
            )

        with open(filename, "w") as f:
            json.dump(data, f, indent=2)

        self.speak(f"已将 {len(self.pose_list)} 个点保存至 {filename}")
        self.get_logger().info(f"JSON 文件已写入: {filename}")

    def load_pose_list_from_json(self, filename="pose_list.json"):
        try:
            if not os.path.exists(filename):
                with open(filename, "w") as f:
                    json.dump([], f)

            with open(filename, "r") as f:
                data = json.load(f)

            self.pose_list.clear()
            for item in data:
                pose = PoseStamped()
                pose.header.frame_id = item["frame_id"]
                pose.pose.position.x = item["position"]["x"]
                pose.pose.position.y = item["position"]["y"]
                pose.pose.position.z = item["position"]["z"]
                pose.pose.orientation.x = item["orientation"]["x"]
                pose.pose.orientation.y = item["orientation"]["y"]
                pose.pose.orientation.z = item["orientation"]["z"]
                pose.pose.orientation.w = item["orientation"]["w"]
                self.pose_list.append(pose)

            self.pose_next = 0
            self.speak(f"成功加载 {len(self.pose_list)} 个点位")
            self.get_logger().info(f"已加载 JSON 文件: {filename}")
        except Exception as e:
            self.get_logger().error(f"加载 JSON 文件失败: {e}")
            self.speak("加载点位失败")

    def move_pose_by_topic(self, pose: PoseStamped):
        """
        发布 /goal_pose，引导机器人前往目标位姿点（包括位置+方向）

        参数:
        - pose: PoseStamped，目标点（从列表中获取或手动构造）
        """
        if pose is None:
            self.get_logger().warn("目标点无效，导航取消")
            return

        self.goal_pose_pub.publish(pose)
        self.speak(
            f"导航到目标点：x={pose.pose.position.x:.2f}, y={pose.pose.position.y:.2f}"
        )
        self.get_logger().info(
            f" 已发布目标位姿: x={pose.pose.position.x:.2f}, y={pose.pose.position.y:.2f}"
        )

    def init_tf(self):
        self.get_logger().info("等待 TF [mic_link -> base_link] 可用...")
        while rclpy.ok():
            rclpy.spin_once(self, timeout_sec=0.1)  #  让 listener 接收到 static TF
            if self.tf_buffer.can_transform("base_link", "mic_link", rclpy.time.Time()):
                try:
                    tf = self.tf_buffer.lookup_transform(
                        "base_link", "mic_link", rclpy.time.Time()
                    )
                    self.get_logger().info("TF 获取成功 ")
                    return tf
                except Exception as e:
                    self.get_logger().warn(f"can_transform通过但lookup失败：{e}")
            else:
                self.get_logger().info("TF 尚未可用，继续等待...")
                time.sleep(0.5)

    def init_commandMatcher(self):
        data = [
            {"key": "保存"},
            {"key": "当前"},
            {"key": "位置"},
            {"key": "清除"},
            {"key": "去"},
            {"key": "前"},
            {"key": "往"},
            {"key": "下"},
            {"key": "一"},
            {"key": "个"},
            {"key": "点"},
            {"key": "上"},
            {"key": "看"},
            {"key": "转"},
            {"key": "我"},
            {"key": "朝向"},
            {"key": "所有"},
            {"key": "下一个"},
            {"key": "上一个"},
            {"key": "前一个"},
            {"key": "返回"},
        ]

        cmd = [
            {
                "cmd": 0x01,
                "match": [
                    ["保存", "当前位置"],
                    ["保存", "位置"],
                    ["保存", "当前"],
                    ["保存", "当前位置点"],
                ],
            },
            {
                "cmd": 0x02,
                "match": [["清除", "所有"], ["清除", "位置"], ["清除", "点位"]],
            },
            {
                "cmd": 0x03,
                "match": [
                    ["去", "下", "个", "点"],
                    ["往", "下", "个", "点"],
                    ["去", "下一个"],
                    ["前往", "下一个", "点"],
                ],
            },
            {
                "cmd": 0x04,
                "match": [
                    ["去", "上", "个", "点"],
                    ["往", "上", "个", "点"],
                    ["返回", "上一个", "点"],
                ],
            },
            {
                "cmd": 0x05,
                "match": [
                    ["看", "我"],
                    ["看看", "我"],
                    ["朝向", "我"],
                    ["转", "我"],
                    ["转向", "我"],
                ],
            },
        ]
        self.commandMatcher = CommandMatcher(data, cmd)

    def init_publishers(self):
        self.voice_dictation_wake_up_topic_pub = self.create_publisher(
            Int8, "/awake_voixe_control_flag", 10
        )
        self.controller_node = self.create_publisher(
            Empty, "/controller_node/dashboard/start", 10
        )
        self.goal_pose_pub = self.create_publisher(PoseStamped, "/goal_pose", 10)

    def init_subscriptions(self):
        self.create_subscription(String, "/xf/voice", self.iat_callback, 10)
        self.create_subscription(Int8, "/awake_flag", self.wakeup_callback, 10)
        self.create_subscription(UInt32, "/awake_angle", self.angle_callback, 10)

    def angle_callback(self, msg):
        mic_angle_deg = msg.data
        theta_rad = math.radians(mic_angle_deg)
        # 麦克风坐标系下的方向向量（XY 平面）
        v_mic = np.array([math.cos(theta_rad), math.sin(theta_rad), 0.0])
        # 提取四元数 → 旋转矩阵
        q = self.tf_transform.transform.rotation
        from scipy.spatial.transform import Rotation as R

        rot = R.from_quat([q.x, q.y, q.z, q.w])
        R_mat = rot.as_matrix()
        # 旋转向量到 base_link 坐标系
        v_base = R_mat @ v_mic

        # 计算 base_link 坐标系下的角度（XY 平面）
        angle_rad = math.atan2(v_base[1], v_base[0])
        self.base_angle = math.degrees(angle_rad) % 360

        self.get_logger().info(
            f"[mic_link] 输入角度：{mic_angle_deg}° → [base_link] 转换后角度：{self.base_angle:.2f}°"
        )

    def iat_callback(self, msg):
        self.get_logger().info(f"接收到识别结果：{msg.data}")
        command = self.commandMatcher.match_command(msg.data)
        self.get_logger().info(f"匹配命令码：{hex(command) if command else 'None'}")

        state = RobotState.from_value(command)
        if state:
            # 如果当前正在语音识别等待中，直接切换到新状态
            if self.robot_current_sate == RobotState.IAT_WAIT:
                self.get_logger().info(
                    f"识别成功，打断语音识别等待，切换状态 -> {state.name}"
                )
                self.robot_current_sate = state
                self.robot_new_sate = RobotState.IDLE  # 清除待处理状态
                self._state_machine_step()  #  强制执行一次状态机逻辑
            else:
                self.robot_new_sate = state

    def wakeup_callback(self, msg):
        self.get_logger().info("收到唤醒词，准备启动语音听写")
        if self.robot_current_sate == RobotState.IDLE:
            self.get_logger().info("机器人空闲，启动语音听写")
            self.robot_new_sate = RobotState.IAT
        else:
            self.get_logger().warn(
                f"当前状态 {self.robot_current_sate.name}，忽略唤醒请求"
            )
            self.speak("我现在正忙，请稍后再试，但是我已对您重新定位")

    def speak(self, text, iat_flag=False):
        req = Xf.Request()
        req.text = text
        future = self.cli.call_async(req)

        def done_callback(fut):
            result = fut.result()
            if result and result.success:
                self.get_logger().info(f"TTS 成功播报：{text}")
                if iat_flag:
                    self.get_logger().info("准备发布语音唤醒指令...")
                    self.pub_voice_dictation_wake_up()
            else:
                self.get_logger().warn(
                    f"TTS 播报失败：{result.message if result else '无响应'}"
                )

        future.add_done_callback(done_callback)

    def run_state_machine(self):
        timer_period = 1.0
        self.timer = self.create_timer(timer_period, self._state_machine_step)

    def _state_machine_step(self):
        now = self.get_clock().now().seconds_nanoseconds()[0]

        if self.robot_current_sate == RobotState.IDLE:
            if self.robot_new_sate != RobotState.IDLE:
                self.get_logger().info(
                    f"机器人状态从 {self.robot_current_sate.name} -> {self.robot_new_sate.name}"
                )
                self.robot_current_sate = self.robot_new_sate
                self.robot_new_sate = RobotState.IDLE
                self.cold_time = now
                self.execution_times = 0
            return

        elif self.robot_current_sate == RobotState.IAT:
            # self.speak("滴滴，请说出指令", iat_flag=True)  # 原提示语
            self.speak("我是具身智能机器人小K, 请您说出指令, 有任何需求请吩咐我", iat_flag=True)
            self.robot_current_sate = RobotState.IAT_WAIT

        elif self.robot_current_sate == RobotState.IAT_WAIT:
            if now - self.cold_time > self.iat_wait_max_time:
                if self.execution_times > self.iat_wait_execution_times:
                    self.speak("超出最大尝试次数，结束识别")
                    self.robot_current_sate = RobotState.IDLE
                else:
                    self.speak("未听清，请再说一次", iat_flag=True)
                    self.execution_times += 1
                    self.cold_time = now

        else:
            # 所有其他命令在此执行一次即可
            self._execute_current_state()
            self.robot_current_sate = RobotState.IDLE

    def _execute_current_state(self):
        state = self.robot_current_sate

        if state == RobotState.LOOK_AT_ME:
            self.look_at_user()
        elif state == RobotState.SAVE_POSE:
            self.speak("保存当前位置")
            self.save_cure_pose_list()
            self.save_pose_list_to_json(
                "/home/rpp//rpp_data/config/voice/voice_pose.json"
            )
        elif state == RobotState.CLEAR_POSE:
            self.speak("清除所有位置")
            self.clear_pose_list()
            # 清除文件中的位置？
        elif state == RobotState.NEXT_POSE:
            self.speak("前往下一个点")
            self.move_pose_addition()
            self.move_pose_by_topic(self.pose_list[self.pose_next])
        elif state == RobotState.PREV_POSE:
            self.speak("返回上一个点")
            self.move_pose_subtraction()
            self.move_pose_by_topic(self.pose_list[self.pose_next])

        # 所有这些命令完成后重置为 IDLE
        self.robot_current_sate = RobotState.IDLE

    def pub_voice_dictation_wake_up(self):
        msg = Int8()
        msg.data = 1
        self.get_logger().info(f"发布唤醒词")
        self.voice_dictation_wake_up_topic_pub.publish(msg)

    def pub_controller_node(self):
        self.controller_node.publish(Empty())

    def set_pick_target_type_param(self, val):
        os.system(f"ros2 param set /pick_target_type {val}")

    def set_pick_pose(self, val):
        os.system(f"ros2 param set /pick_pose {val}")

    def look_at_user(self):
        """
        根据 self.base_angle 直接原地转向用户方向（让其在正前方）
        """
        if self.base_angle is None:
            self.get_logger().warn("尚未获取 base_angle，无法转向")
            return

        # 选择最短旋转路径
        angle = self.base_angle
        if angle < 5:
            self.speak("我已经看着您了")
            return
        if angle < 180:
            direction = "rotate_left"
            rotate_angle = angle
        else:
            direction = "rotate_right"
            rotate_angle = 360 - angle

        self.get_logger().info(
            f"准备朝向用户方向，方向: {direction}, 角度: {rotate_angle:.2f}°"
        )
        self.speak("正在朝向您")

        self.start_rotation(direction, rotate_angle, 30)

    def start_rotation(self, direction, angle_deg, speed_deg_per_sec):
        self.rotation_active = True
        self.rotation_direction = direction
        self.rotation_target_angle = angle_deg
        self.rotation_speed = speed_deg_per_sec
        self.rotation_start_time = time.time()
        self.rotation_pub = self.create_publisher(Twist, "/cmd_vel", 10)
        self.rotation_timer = self.create_timer(0.1, self.rotation_step)

    def rotation_step(self):
        if not self.rotation_active:
            return
        elapsed = time.time() - self.rotation_start_time
        moved = self.rotation_speed * elapsed
        if moved >= self.rotation_target_angle:
            stop_msg = Twist()
            self.rotation_pub.publish(stop_msg)
            self.rotation_active = False
            self.rotation_timer.cancel()
            self.get_logger().info("✅ 旋转完成")
            self.speak("转向完成")
        else:
            twist = Twist()
            angular_speed = math.radians(self.rotation_speed)
            if self.rotation_direction == "rotate_left":
                twist.angular.z = angular_speed
            elif self.rotation_direction == "rotate_right":
                twist.angular.z = -angular_speed
            self.rotation_pub.publish(twist)

    def get_yaw_from_pose(self, pose_stamped):
        q = pose_stamped.pose.orientation
        quat = [q.x, q.y, q.z, q.w]
        roll, pitch, yaw = euler_from_quaternion(quat)
        yaw_deg = math.degrees(yaw) % 360
        return yaw_deg

    def get_current_pose_from_tf(
        self, tf_buffer, node, target_frame="map", source_frame="base_link"
    ):
        """
        从 TF 中获取机器人当前位姿（PoseStamped）

        参数:
        - tf_buffer: 已初始化的 tf2_ros.Buffer 对象
        - node: 当前 ROS2 节点（用于时间、日志）
        - target_frame: 通常为 'map' 或 'odom'
        - source_frame: 通常为 'base_link'

        返回:
        - PoseStamped 对象，包含机器人在 map 下的位置和朝向
        """
        try:
            # 获取当前时刻的变换
            transform = tf_buffer.lookup_transform(
                target_frame, source_frame, rclpy.time.Time()
            )

            # 位置
            trans = transform.transform.translation
            rot = transform.transform.rotation

            # 构造 PoseStamped
            pose = PoseStamped()
            pose.header.stamp = node.get_clock().now().to_msg()
            pose.header.frame_id = target_frame

            pose.pose.position.x = trans.x
            pose.pose.position.y = trans.y
            pose.pose.position.z = trans.z

            pose.pose.orientation = rot

            return pose

        except Exception as e:
            node.get_logger().error(
                f"[TF] 获取 {source_frame} 在 {target_frame} 下的位姿失败: {e}"
            )
            return None


def main(args=None):
    rclpy.init(args=args)
    node = VoiceControlNode()
    node.tf_transform = node.init_tf()
    node.run_state_machine()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
