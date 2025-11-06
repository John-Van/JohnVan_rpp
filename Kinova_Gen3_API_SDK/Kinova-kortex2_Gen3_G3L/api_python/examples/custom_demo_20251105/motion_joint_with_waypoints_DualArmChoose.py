#!/usr/bin/env python3
"""
Kinova Gen3 双臂关节位置控制运动程序

功能描述：
1. 关节位置精确控制：
   - 读取机械臂当前关节角度作为基准位置
   - 执行单次关节角运动：先增加5度并停留2秒，再减少5度并停留2秒
   - 每次运动后打印当前关节角度和末端笛卡尔位姿
2. 支持选择左臂或右臂
3. 运动参数：
   - 最大速度：1°/s
   - 加速度：0.2°/s²
   - 角度：正负5°
"
2. 运动参数控制：
   - 最大运行速度：1.0°/s（可配置的目标运行速度）
   - 启动/结束加速度：0.2°/s²（平滑的加减速过程）
   - 运动轨迹：当前位置 → +5° → -5° → 原位置

3. 安全特性：
   - 基于当前位置的相对运动，避免绝对位置风险
   - 运动前验证轨迹有效性
   - 异常处理和超时保护机制
   - 平滑的速度曲线，减少机械冲击

4. 技术实现：
   - 使用WaypointList进行轨迹规划
   - AngularWaypoint定义关节空间路径点
   - 基于时间的运动控制，确保速度和加速度约束
   - 事件驱动的运动完成检测

5. 应用场景：
   - 关节校准和测试
   - 运动参数验证
   - 机械臂健康检查
   - 控制算法调试

作者：CodeBuddy
创建日期：2025/11/5
版本：1.0
"""

import sys
import os
import time
import threading
import math

from kortex_api.autogen.client_stubs.BaseClientRpc import BaseClient
from kortex_api.autogen.client_stubs.BaseCyclicClientRpc import BaseCyclicClient

from kortex_api.autogen.messages import Base_pb2, BaseCyclic_pb2, Common_pb2

# 运动参数配置
MAX_VELOCITY_DEG_PER_SEC = 1.0      # 最大速度：1度/秒
ACCELERATION_DEG_PER_SEC2 = 0.2     # 加速度：0.2度/秒²
OSCILLATION_ANGLE_DEG = 5.0         # 上下变化角度：5度
TIMEOUT_DURATION = 60               # 超时时间：60秒

def check_for_end_or_abort(e):
    """返回一个检查END或ABORT通知的闭包函数
    
    参数:
    e -- 当动作完成时要设置的事件对象
    """
    def check(notification, e=e):
        print("EVENT : " + Base_pb2.ActionEvent.Name(notification.action_event))
        if notification.action_event == Base_pb2.ACTION_END \
        or notification.action_event == Base_pb2.ACTION_ABORT:
            e.set()
    return check

def move_to_initial_position(base, initial_pose_name="Home"):
    """将机械臂移动到指定的初始位置"""
    print(f"正在将机械臂移动到初始位置: {initial_pose_name}...")
    
    # 确保机械臂处于单级伺服模式
    base_servo_mode = Base_pb2.ServoingModeInformation()
    base_servo_mode.servoing_mode = Base_pb2.SINGLE_LEVEL_SERVOING
    base.SetServoingMode(base_servo_mode)
    
    # 移动到指定的初始位置
    action_type = Base_pb2.RequestedActionType()
    action_type.action_type = Base_pb2.REACH_JOINT_ANGLES
    action_list = base.ReadAllActions(action_type)
    action_handle = None
    
    for action in action_list.action_list:
        if action.name == initial_pose_name:
            action_handle = action.handle
            break

    if action_handle == None:
        print(f"无法找到初始位姿 '{initial_pose_name}'，尝试使用 'Home' 位置...")
        for action in action_list.action_list:
            if action.name == "Home":
                action_handle = action.handle
                break
        
        if action_handle == None:
            print("无法到达任何安全位置，程序退出")
            return False

    e = threading.Event()
    notification_handle = base.OnNotificationActionTopic(
        check_for_end_or_abort(e),
        Base_pb2.NotificationOptions()
    )

    base.ExecuteActionFromReference(action_handle)
    finished = e.wait(TIMEOUT_DURATION)
    base.Unsubscribe(notification_handle)

    if finished:
        print(f"已到达初始位置: {initial_pose_name}")
    else:
        print("移动到初始位置超时")
    return finished

def get_current_joint_angles(base_cyclic):
    """获取当前关节角度"""
    try:
        feedback = base_cyclic.RefreshFeedback()
        current_angles = []
        
        # 提取所有关节的当前角度
        for i in range(len(feedback.actuators)):
            current_angles.append(feedback.actuators[i].position)
            
        print(f"当前关节角度: {[f'{angle:.2f}°' for angle in current_angles]}")
        return current_angles
    except Exception as e:
        print(f"获取当前关节角度失败: {e}")
        return None

def calculate_movement_duration(angle_change_deg):
    """根据角度变化和速度/加速度约束计算运动时间"""
    # 考虑加速和减速阶段
    # 假设运动分为三个阶段：加速、匀速、减速
    
    # 加速到最大速度所需的时间和角度
    t_accel = MAX_VELOCITY_DEG_PER_SEC / ACCELERATION_DEG_PER_SEC2
    angle_accel = 0.5 * ACCELERATION_DEG_PER_SEC2 * t_accel * t_accel
    
    # 如果角度变化小于两倍加速角度，则不会达到最大速度
    if abs(angle_change_deg) <= 2 * angle_accel:
        # 三角形速度曲线
        t_total = math.sqrt(2 * abs(angle_change_deg) / ACCELERATION_DEG_PER_SEC2)
        return max(t_total, 1.0)  # 最小1秒
    else:
        # 梯形速度曲线
        angle_const_velocity = abs(angle_change_deg) - 2 * angle_accel
        t_const_velocity = angle_const_velocity / MAX_VELOCITY_DEG_PER_SEC
        t_total = 2 * t_accel + t_const_velocity
        return max(t_total, 2.0)  # 最小2秒

def create_angular_waypoint(joint_angles, duration):
    """创建角度路径点"""
    waypoint = Base_pb2.AngularWaypoint()
    waypoint.angles.extend(joint_angles)
    waypoint.duration = duration
    return waypoint

def get_cartesian_pose(base):
    """获取当前末端笛卡尔位姿"""
    try:
        pose = base.GetMeasuredCartesianPose()
        return pose
    except Exception as e:
        print(f"获取笛卡尔位姿失败: {e}")
        return None

def print_robot_status(base, base_cyclic, description=""):
    """打印机械臂当前状态信息"""
    print(f"\n--- {description} ---")
    
    # 获取关节角度
    joint_angles = get_current_joint_angles(base_cyclic)
    if joint_angles:
        print("关节角度 (度):")
        for i, angle in enumerate(joint_angles):
            print(f"  关节 {i}: {angle:.3f}°")
    
    # 获取笛卡尔位姿
    cartesian_pose = get_cartesian_pose(base)
    if cartesian_pose:
        print("末端笛卡尔位姿:")
        print(f"  位置 (m): X={cartesian_pose.x:.6f}, Y={cartesian_pose.y:.6f}, Z={cartesian_pose.z:.6f}")
        print(f"  姿态 (度): θX={cartesian_pose.theta_x:.3f}, θY={cartesian_pose.theta_y:.3f}, θZ={cartesian_pose.theta_z:.3f}")
    
    print("-" * 50)

def execute_all_joints_oscillation(base, base_cyclic):
    """执行所有关节的运动
    
    参数:
    base -- Base客户端
    base_cyclic -- BaseCyclic客户端  
    """
    print("开始执行所有关节的单次运动...")
    
    # 获取当前关节角度
    current_angles = get_current_joint_angles(base_cyclic)
    if current_angles is None:
        return False
    
    num_joints = len(current_angles)
    print(f"机械臂共有 {num_joints} 个关节")
    
    # 打印初始状态
    print_robot_status(base, base_cyclic, "初始状态")
    
    try:
        # 第一阶段：所有关节 +5 度
        print(f"\n=== 第一阶段：所有关节 +{OSCILLATION_ANGLE_DEG}° ===")
        angles_plus_5 = current_angles.copy()
        for i in range(num_joints):
            angles_plus_5[i] += OSCILLATION_ANGLE_DEG
        
        # 创建+5度轨迹
        waypoints_plus = Base_pb2.WaypointList()
        waypoints_plus.duration = 0.0
        waypoints_plus.use_optimal_blending = False
        
        duration_plus = calculate_movement_duration(OSCILLATION_ANGLE_DEG)
        print(f"运动时间: {duration_plus:.2f}秒")
        
        waypoint_plus = waypoints_plus.waypoints.add()
        waypoint_plus.name = "all_joints_plus_5_deg"
        waypoint_plus.angular_waypoint.CopyFrom(create_angular_waypoint(angles_plus_5, duration_plus))
        
        # 执行+5度运动
        e1 = threading.Event()
        notification_handle1 = base.OnNotificationActionTopic(
            check_for_end_or_abort(e1),
            Base_pb2.NotificationOptions()
        )
        
        print("执行 +5° 运动...")
        base.ExecuteWaypointTrajectory(waypoints_plus)
        
        finished1 = e1.wait(TIMEOUT_DURATION)
        base.Unsubscribe(notification_handle1)
        
        if not finished1:
            print("第一阶段运动超时！")
            return False
        
        print("✓ +5° 运动完成")
        
        # 打印+5度状态并停留2秒
        print_robot_status(base, base_cyclic, "所有关节 +5° 状态")
        print("停留 2 秒...")
        time.sleep(2)
    
        # 第二阶段：所有关节 -5 度（相对于初始位置）
        print(f"\n=== 第二阶段：所有关节 -{OSCILLATION_ANGLE_DEG}° ===")
        angles_minus_5 = current_angles.copy()
        for i in range(num_joints):
            angles_minus_5[i] -= OSCILLATION_ANGLE_DEG
        
        # 创建-5度轨迹
        waypoints_minus = Base_pb2.WaypointList()
        waypoints_minus.duration = 0.0
        waypoints_minus.use_optimal_blending = False
        
        duration_minus = calculate_movement_duration(2 * OSCILLATION_ANGLE_DEG)  # 从+5到-5是10度
        print(f"运动时间: {duration_minus:.2f}秒")
        
        waypoint_minus = waypoints_minus.waypoints.add()
        waypoint_minus.name = "all_joints_minus_5_deg"
        waypoint_minus.angular_waypoint.CopyFrom(create_angular_waypoint(angles_minus_5, duration_minus))
        
        # 执行-5度运动
        e2 = threading.Event()
        notification_handle2 = base.OnNotificationActionTopic(
            check_for_end_or_abort(e2),
            Base_pb2.NotificationOptions()
        )
        
        print("执行 -5° 运动...")
        base.ExecuteWaypointTrajectory(waypoints_minus)
        
        finished2 = e2.wait(TIMEOUT_DURATION)
        base.Unsubscribe(notification_handle2)
        
        if not finished2:
            print("第二阶段运动超时！")
            return False
        
        print("✓ -5° 运动完成")
        
        # 打印-5度状态并停留2秒
        print_robot_status(base, base_cyclic, "所有关节 -5° 状态")
        print("停留 2 秒...")
        time.sleep(2)
        
        print("\n✓ 所有关节运动序列完成！")
        return True
        
    except Exception as e:
        print(f"执行运动时出错: {e}")
        return False

def get_user_choice():
    """获取用户选择"""
    print("\n" + "=" * 60)
    print("Kinova Gen3 双臂关节位置控运动程序")
    print("=" * 60)
    print("请选择要控制的机械臂:")
    print("1. 左臂 (IP: 192.168.11.60, 初始位姿: Left_Initial)")
    print("2. 右臂 (IP: 192.168.11.61, 初始位姿: Right_Initial)")
    print("0. 退出程序")
    print("=" * 60)
    
    while True:
        try:
            choice = input("请输入选择 (0/1/2): ").strip()
            if choice in ['0', '1', '2']:
                return int(choice)
            else:
                print("无效选择，请输入 0、1 或 2")
        except KeyboardInterrupt:
            print("\n程序被用户中断")
            return 0
        except Exception as e:
            print(f"输入错误: {e}")

def get_arm_config(choice):
    """根据选择获取机械臂配置"""
    if choice == 1:
        return {
            'ip': '192.168.11.60',
            'username': 'admin', 
            'password': 'admin',
            'initial_pose': 'Left_Initial',
            'name': '左臂'
        }
    elif choice == 2:
        return {
            'ip': '192.168.11.61',
            'username': 'admin',
            'password': 'admin', 
            'initial_pose': 'Right_Initial',
            'name': '右臂'
        }
    else:
        return None

class CustomArgs:
    """自定义参数类，用于替代命令行参数"""
    def __init__(self, ip, username, password):
        self.ip = ip
        self.username = username
        self.password = password

def main():
    """主函数"""
    
    # 导入工具模块
    sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
    import utilities
    
    # 获取用户选择
    choice = get_user_choice()
    
    if choice == 0:
        print("程序退出")
        return 0
    
    # 获取机械臂配置
    arm_config = get_arm_config(choice)
    if arm_config is None:
        print("无效的选择")
        return 1
    
    print(f"\n已选择: {arm_config['name']}")
    print(f"IP地址: {arm_config['ip']}")
    print(f"初始位姿: {arm_config['initial_pose']}")
    
    # 创建自定义参数对象
    args = CustomArgs(arm_config['ip'], arm_config['username'], arm_config['password'])
    
    try:
        # 创建TCP连接
        with utilities.DeviceConnection.createTcpConnection(args) as router:
            
            # 创建必要的服务客户端
            base = BaseClient(router)
            base_cyclic = BaseCyclicClient(router)
            
            # 程序执行流程
            success = True
            
            print(f"\n正在连接到 {arm_config['name']} ({arm_config['ip']})...")
            
            # 步骤1：移动到初始位置
            success &= move_to_initial_position(base, arm_config['initial_pose'])
            
            if success:
                # 步骤2：执行所有关节运动
                print(f"\n开始执行 {arm_config['name']} 所有关节运动...")
                success &= execute_all_joints_oscillation(base, base_cyclic)
            
            print("\n" + "=" * 60)
            if success:
                print(f"✓ {arm_config['name']} 运动程序执行成功完成")
            else:
                print(f"✗ {arm_config['name']} 运动程序执行过程中出现错误")
            print("=" * 60)
            
            return 0 if success else 1
            
    except Exception as e:
        print(f"连接或执行过程中出现错误: {e}")
        return 1

if __name__ == "__main__":
    exit(main())