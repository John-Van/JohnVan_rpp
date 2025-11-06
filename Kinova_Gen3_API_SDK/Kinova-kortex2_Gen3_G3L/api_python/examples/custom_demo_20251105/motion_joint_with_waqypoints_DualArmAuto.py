#!/usr/bin/env python3
"""
Kinova Gen3 双臂关节控制运动程序（带速度限制）

功能描述：
1. 读取左臂和右臂的当前状态并打印。
2. 依次控制双臂运动（±5°），速度严格限制在1°/s。
3. 运动完成后打印状态并停留，最终返回初始位置。
"""

import sys
import os
import time
import threading
import math

from kortex_api.autogen.client_stubs.BaseClientRpc import BaseClient
from kortex_api.autogen.client_stubs.BaseCyclicClientRpc import BaseCyclicClient

from kortex_api.autogen.messages import Base_pb2, BaseCyclic_pb2, Common_pb2

# 运动参数配置（核心：速度限制）
MAX_VELOCITY_DEG_PER_SEC = 1.0      # 最大速度：1度/秒（关键参数）
ACCELERATION_DEG_PER_SEC2 = 0.2     # 加速度：0.2度/秒²
OSCILLATION_ANGLE_DEG = 5.0         # 变化角度：5度
TIMEOUT_DURATION = 60               # 超时时间：60秒

# 双臂配置
LEFT_ARM_IP = "192.168.11.60"
RIGHT_ARM_IP = "192.168.11.61"
USERNAME = "admin"
PASSWORD = "admin"

def check_for_end_or_abort(e):
    """检查动作结束或中止的通知"""
    def check(notification, e=e):
        print(f"EVENT : {Base_pb2.ActionEvent.Name(notification.action_event)}")
        if notification.action_event in [Base_pb2.ACTION_END, Base_pb2.ACTION_ABORT]:
            e.set()
    return check

def get_current_joint_angles(base_cyclic):
    """获取当前关节角度"""
    try:
        feedback = base_cyclic.RefreshFeedback()
        current_angles = [act.position for act in feedback.actuators]
        print(f"当前关节角度: {[f'{angle:.2f}°' for angle in current_angles]}")
        return current_angles
    except Exception as e:
        print(f"获取当前关节角度失败: {e}")
        return None

def get_cartesian_pose(base):
    """获取当前末端笛卡尔位姿"""
    try:
        return base.GetMeasuredCartesianPose()
    except Exception as e:
        print(f"获取笛卡尔位姿失败: {e}")
        return None

def print_robot_status(base, base_cyclic, description=""):
    """打印机械臂当前状态信息"""
    print(f"\n--- {description} ---")
    
    # 打印关节角度
    joint_angles = get_current_joint_angles(base_cyclic)
    if joint_angles:
        for i, angle in enumerate(joint_angles):
            print(f"  关节 {i}: {angle:.3f}°")
    
    # 打印笛卡尔位姿
    cartesian_pose = get_cartesian_pose(base)
    if cartesian_pose:
        print("末端笛卡尔位姿:")
        print(f"  位置 (m): X={cartesian_pose.x:.6f}, Y={cartesian_pose.y:.6f}, Z={cartesian_pose.z:.6f}")
        print(f"  姿态 (度): θX={cartesian_pose.theta_x:.3f}, θY={cartesian_pose.theta_y:.3f}, θZ={cartesian_pose.theta_z:.3f}")
    
    print("-" * 50)

def calculate_duration(current_angles, target_angles, max_velocity):
    """计算运动时间（确保速度不超过max_velocity）"""
    max_angle_change = max(
        abs(target - current) 
        for target, current in zip(target_angles, current_angles)
    )
    return max_angle_change / max_velocity  # 时间 = 最大角度变化 / 最大速度

def create_waypoint_trajectory(target_angles, duration):
    """创建带速度约束的轨迹点列表"""
    waypoints = Base_pb2.WaypointList()
    waypoints.duration = 0.0  # 整体轨迹持续时间（0表示使用各点自身duration）
    waypoints.use_optimal_blending = False  # 关闭轨迹混合，精确控制速度
    
    # 添加目标轨迹点
    waypoint = waypoints.waypoints.add()
    waypoint.name = "velocity_constrained_move"
    
    # 设置关节角度和运动时间（核心：通过duration控制速度）
    angular_waypoint = Base_pb2.AngularWaypoint()
    angular_waypoint.angles.extend(target_angles)
    angular_waypoint.duration = duration  # 运动时间（秒）
    waypoint.angular_waypoint.CopyFrom(angular_waypoint)
    
    return waypoints

def move_joints(base, base_cyclic, target_angles, description, max_velocity):
    """控制机械臂按指定速度运动到目标关节角度（核心修改）"""
    print(f"\n=== {description} ===")
    
    # 获取当前角度，计算运动时间
    current_angles = get_current_joint_angles(base_cyclic)
    if not current_angles:
        return False
    
    # 计算运动时间（确保速度不超过限制）
    duration = calculate_duration(current_angles, target_angles, max_velocity)
    print(f"运动参数: 最大角度变化={max(abs(t-c) for t,c in zip(target_angles, current_angles)):.2f}°")
    print(f"          限制速度={max_velocity:.2f}°/s, 预计耗时={duration:.2f}秒")
    
    # 创建带速度约束的轨迹
    waypoints = create_waypoint_trajectory(target_angles, duration)
    
    # 验证轨迹有效性（防止速度超限或路径错误）
    try:
        validation = base.ValidateWaypointList(waypoints)
        if validation.trajectory_error_report.trajectory_error_elements:
            print("轨迹验证失败，可能速度超限或路径无效")
            return False
    except Exception as e:
        print(f"轨迹验证出错: {e}")
        return False
    
    # 执行轨迹运动（替换原有ExecuteAction）
    e = threading.Event()
    notification_handle = base.OnNotificationActionTopic(
        check_for_end_or_abort(e),
        Base_pb2.NotificationOptions()
    )
    
    print(f"执行 {description} 运动...")
    base.ExecuteWaypointTrajectory(waypoints)  # 关键：使用轨迹点执行，而非默认动作
    
    # 等待运动完成
    finished = e.wait(TIMEOUT_DURATION)
    base.Unsubscribe(notification_handle)
    
    if finished:
        print(f"✓ {description} 运动完成（实际耗时与预计一致则速度控制生效）")
    else:
        print(f"✗ {description} 运动超时")
    
    return finished

def control_arm(base, base_cyclic, arm_name, angle_change):
    """控制单臂运动（传入速度限制参数）"""
    current_angles = get_current_joint_angles(base_cyclic)
    if not current_angles:
        return False
    
    # 计算目标角度
    target_angles = [angle + angle_change for angle in current_angles]
    
    # 执行运动（传入最大速度限制）
    description = f"{arm_name} 关节 {'增加' if angle_change > 0 else '减少'} {abs(angle_change)}°"
    success = move_joints(
        base, 
        base_cyclic, 
        target_angles, 
        description, 
        MAX_VELOCITY_DEG_PER_SEC  # 应用速度限制
    )
    
    # 打印状态并停留
    if success:
        print_robot_status(base, base_cyclic, f"{arm_name} 当前状态")
        print("停留 1 秒...")
        time.sleep(1)
    
    return success

class CustomArgs:
    """自定义参数类，用于替代命令行参数"""
    def __init__(self, ip, username, password):
        self.ip = ip
        self.username = username
        self.password = password

def main():
    """主函数"""
    sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
    import utilities
    
    try:
        # 连接左臂
        print("\n正在连接左臂...")
        left_args = CustomArgs(LEFT_ARM_IP, USERNAME, PASSWORD)
        with utilities.DeviceConnection.createTcpConnection(left_args) as left_router:
            left_base = BaseClient(left_router)
            left_base_cyclic = BaseCyclicClient(left_router)
            
            # 连接右臂
            print("\n正在连接右臂...")
            right_args = CustomArgs(RIGHT_ARM_IP, USERNAME, PASSWORD)
            with utilities.DeviceConnection.createTcpConnection(right_args) as right_router:
                right_base = BaseClient(right_router)
                right_base_cyclic = BaseCyclicClient(right_router)
                
                # 打印初始状态
                print("\n--- 初始状态 ---")
                print_robot_status(left_base, left_base_cyclic, "左臂初始状态")
                print_robot_status(right_base, right_base_cyclic, "右臂初始状态")
                
                # 控制左臂增加5°（速度限制1°/s）
                if not control_arm(left_base, left_base_cyclic, "左臂", OSCILLATION_ANGLE_DEG):
                    return 1
                
                # 控制右臂增加5°（速度限制1°/s）
                if not control_arm(right_base, right_base_cyclic, "右臂", OSCILLATION_ANGLE_DEG):
                    return 1
                
                # 控制左臂减少5°（速度限制1°/s）
                if not control_arm(left_base, left_base_cyclic, "左臂", -OSCILLATION_ANGLE_DEG):
                    return 1
                
                # 控制右臂减少5°（速度限制1°/s）
                if not control_arm(right_base, right_base_cyclic, "右臂", -OSCILLATION_ANGLE_DEG):
                    return 1
                
                print("\n✓ 已完成全部运动，退出")
                return 0
    except Exception as e:
        print(f"连接或执行过程中出现错误: {e}")
        return 1

if __name__ == "__main__":
    exit(main())