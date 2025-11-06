#!/usr/bin/env python3
"""
机械臂双臂笛卡尔空间运动控制程序
Kinova Gen3 双臂笛卡尔空间运动控制程序

功能描述：
1. 运行前选择控制左臂或右臂
2. 通过ExecuteAction动作接口控制机械臂在笛卡尔空间运动
3. 位置先增加(0.05, 0.05, 0.05)m，姿态增加小角度
4. 运动前后打印关节位置和位姿信息
5. 两次运动中间停顿2秒

技术实现：
1. 使用Base_pb2.Action创建笛卡尔位姿运动
2. 通过ExecuteAction执行预定义动作
3. 获取并打印关节角度和笛卡尔位姿

使用说明：
- 左臂IP: 192.168.11.60
- 右臂IP: 192.168.11.61
- 账户: admin
- 密码: admin

作者：Craft
创建日期：2025-11-06
"""

import time
import sys
import os
import threading
import math

# 添加路径以便导入Kortex API和utilities模块
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.dirname(current_dir)
sys.path.insert(0, parent_dir)
sys.path.insert(0, os.path.join(parent_dir, "..", ".."))

from kortex_api.autogen.client_stubs.BaseClientRpc import BaseClient
from kortex_api.autogen.client_stubs.BaseCyclicClientRpc import BaseCyclicClient
from kortex_api.autogen.messages import Base_pb2, BaseCyclic_pb2, Common_pb2

import utilities

# 运动超时时间（秒）
TIMEOUT_DURATION = 30

# 双臂连接参数
LEFT_ARM_IP = "192.168.11.60"
RIGHT_ARM_IP = "192.168.11.61"
USERNAME = "admin"
PASSWORD = "admin"

def check_for_end_or_abort(e):
    """检查动作是否完成或中止"""
    def check(notif, e=e):
        if notif.action_event == Base_pb2.ACTION_END or notif.action_event == Base_pb2.ACTION_ABORT:
            e.set()
    return check

def execute_cartesian_movement(base, base_cyclic, target_pose, movement_name):
    """执行笛卡尔空间运动"""
    
    print(f"执行 {movement_name}...")
    
    # 创建笛卡尔位姿动作
    action = Base_pb2.Action()
    action.name = movement_name
    action.application_data = ""
    
    # 设置目标位姿
    cartesian_pose = action.reach_pose.target_pose
    cartesian_pose.x = target_pose['x']
    cartesian_pose.y = target_pose['y']
    cartesian_pose.z = target_pose['z']
    cartesian_pose.theta_x = target_pose['theta_x']
    cartesian_pose.theta_y = target_pose['theta_y']
    cartesian_pose.theta_z = target_pose['theta_z']
    
    # 设置事件监听
    e = threading.Event()
    notification_handle = base.OnNotificationActionTopic(
        check_for_end_or_abort(e),
        Base_pb2.NotificationOptions()
    )
    
    # 执行动作
    base.ExecuteAction(action)
    
    # 等待运动完成
    finished = e.wait(TIMEOUT_DURATION)
    base.Unsubscribe(notification_handle)
    
    if finished:
        print(f"{movement_name} 完成")
    else:
        print(f"{movement_name} 超时")
    
    return finished

def move_to_initial_position(base, arm_name):
    """移动到初始位置"""
    
    print(f"移动{arm_name}到初始位姿...")
    
    # 确保在单级伺服模式下
    base_servo_mode = Base_pb2.ServoingModeInformation()
    base_servo_mode.servoing_mode = Base_pb2.SINGLE_LEVEL_SERVOING
    base.SetServoingMode(base_servo_mode)
    
    # 查找初始动作
    action_type = Base_pb2.RequestedActionType()
    action_type.action_type = Base_pb2.REACH_JOINT_ANGLES
    action_list = base.ReadAllActions(action_type)
    action_handle = None
    
    # 打印所有可用的动作名称用于调试
    print(f"可用的动作列表:")
    for action in action_list.action_list:
        print(f"  - {action.name}")
    
    # 根据臂名称选择动作
    if arm_name == "左臂":
        target_action = "Left_Initial"
    else:
        target_action = "Right_Initial"
    
    for action in action_list.action_list:
        if action.name == target_action:
            action_handle = action.handle
            print(f"找到目标动作: {target_action}")
            break
    
    if action_handle is None:
        print(f"未找到{target_action}动作，尝试查找Home动作作为备选")
        for action in action_list.action_list:
            if action.name == "Home":
                action_handle = action.handle
                print("使用Home动作作为备选")
                break
    
    if action_handle is None:
        print(f"未找到{target_action}或Home动作，跳过初始位置移动")
        return True
    
    # 执行初始位置动作
    e = threading.Event()
    notification_handle = base.OnNotificationActionTopic(
        check_for_end_or_abort(e),
        Base_pb2.NotificationOptions()
    )
    
    base.ExecuteActionFromReference(action_handle)
    finished = e.wait(TIMEOUT_DURATION)
    base.Unsubscribe(notification_handle)
    
    if finished:
        print("初始位置移动完成")
    else:
        print("初始位置移动超时")
    
    return finished

def get_joint_angles_and_pose(base, base_cyclic):
    """获取当前关节角度和位姿"""
    
    try:
        # 获取关节角度
        joint_angles = base.GetMeasuredJointAngles()
        
        # 检查关节角度数据是否有效
        if not hasattr(joint_angles, 'joint_angles') or len(joint_angles.joint_angles) == 0:
            raise Exception("关节角度数据无效")
            
        angles = [angle.value for angle in joint_angles.joint_angles]
        
        # 获取笛卡尔位姿
        pose = base.GetMeasuredCartesianPose()
        
        # 检查位姿数据是否有效
        if not hasattr(pose, 'x') or not hasattr(pose, 'y') or not hasattr(pose, 'z'):
            raise Exception("位姿数据不完整")
        
        # 获取反馈数据用于theta角度
        feedback = base_cyclic.RefreshFeedback()
        
        # 返回4个值以匹配调用代码的期望
        return angles, pose, pose, feedback
    except Exception as e:
        print(f"获取位姿信息时出错: {e}")
        # 尝试只获取关节角度
        try:
            joint_angles = base.GetMeasuredJointAngles()
            if hasattr(joint_angles, 'joint_angles') and len(joint_angles.joint_angles) > 0:
                angles = [angle.value for angle in joint_angles.joint_angles]
                # 创建默认反馈数据
                feedback = type('Feedback', (), {
                    'base': type('Base', (), {
                        'tool_pose_x': 0.0, 'tool_pose_y': 0.0, 'tool_pose_z': 0.0,
                        'tool_pose_theta_x': 0.0, 'tool_pose_theta_y': 0.0, 'tool_pose_theta_z': 0.0
                    })()
                })()
                # 创建默认位姿
                pose = type('Pose', (), {
                    'x': 0.0, 'y': 0.0, 'z': 0.0,
                    'theta_x': 0.0, 'theta_y': 0.0, 'theta_z': 0.0
                })()
                return angles, pose, pose, feedback
        except:
            pass
        # 返回默认值
        feedback = type('Feedback', (), {
            'base': type('Base', (), {
                'tool_pose_x': 0.0, 'tool_pose_y': 0.0, 'tool_pose_z': 0.0,
                'tool_pose_theta_x': 0.0, 'tool_pose_theta_y': 0.0, 'tool_pose_theta_z': 0.0
            })()
        })()
        pose = type('Pose', (), {
            'x': 0.0, 'y': 0.0, 'z': 0.0,
            'theta_x': 0.0, 'theta_y': 0.0, 'theta_z': 0.0
        })()
        return [0.0] * 7, pose, pose, feedback

def print_joint_angles_and_pose(angles, position, orientation, feedback, prefix=""):
    """打印关节角度和位姿信息"""
    
    print(f"{prefix}关节角度 (度):")
    for i, angle in enumerate(angles):
        print(f"  关节{i+1}: {angle:.2f}°")
    
    print(f"{prefix}笛卡尔位姿:")
    print(f"  位置 (x, y, z): ({position.x:.3f}, {position.y:.3f}, {position.z:.3f}) m")
    print(f"  姿态 (theta_x, theta_y, theta_z): ({orientation.theta_x:.3f}, {orientation.theta_y:.3f}, {orientation.theta_z:.3f}) rad")
    print(f"  Theta角度 (theta_x, theta_y, theta_z): ({feedback.base.tool_pose_theta_x:.2f}, {feedback.base.tool_pose_theta_y:.2f}, {feedback.base.tool_pose_theta_z:.2f})°")

def select_arm():
    """选择要控制的机械臂"""
    
    print("=== 机械臂双臂笛卡尔空间运动控制程序 ===")
    print("请选择要控制的机械臂:")
    print("1. 左臂 (IP: 192.168.11.60)")
    print("2. 右臂 (IP: 192.168.11.61)")
    
    while True:
        try:
            choice = input("请输入选择 (1 或 2): ").strip()
            if choice == "1":
                return "左臂", LEFT_ARM_IP
            elif choice == "2":
                return "右臂", RIGHT_ARM_IP
            else:
                print("无效选择，请输入 1 或 2")
        except KeyboardInterrupt:
            print("\n程序被用户中断")
            sys.exit(0)

def main():
    
    # 选择要控制的机械臂
    arm_name, arm_ip = select_arm()
    
    print(f"\n目标IP: {arm_ip}")
    print(f"控制对象: {arm_name}")
    
    # 创建连接参数
    args = utilities.parseConnectionArguments()
    args.ip = arm_ip
    args.username = USERNAME
    args.password = PASSWORD
    
    try:
        # 创建连接
        with utilities.DeviceConnection.createTcpConnection(args) as router:
            
            # 创建服务客户端
            base = BaseClient(router)
            base_cyclic = BaseCyclicClient(router)
            
            # 移动到初始位置
            if not move_to_initial_position(base, arm_name):
                print("初始位置移动失败，程序退出")
                return 1
            
            # 获取初始关节角度和位姿
            print("\n初始状态:")
            initial_angles, initial_position, initial_orientation, initial_feedback = get_joint_angles_and_pose(base, base_cyclic)
            print_joint_angles_and_pose(initial_angles, initial_position, initial_orientation, initial_feedback, "初始")
            
            # 停顿2秒
            print("\n停顿2秒...")
            time.sleep(2)
            
            # 计算增加(0.02, 0.02, 0.02)m后的目标位姿（减小幅度避免碰撞）
            target_pose_increased = {
                'x': initial_feedback.base.tool_pose_x + 0.02,
                'y': initial_feedback.base.tool_pose_y + 0.02,
                'z': initial_feedback.base.tool_pose_z + 0.02,
                'theta_x': initial_feedback.base.tool_pose_theta_x + 2.0,  # 增加2度
                'theta_y': initial_feedback.base.tool_pose_theta_y + 2.0,  # 增加2度
                'theta_z': initial_feedback.base.tool_pose_theta_z + 2.0   # 增加2度
            }
            
            # 执行增加位姿运动
            if not execute_cartesian_movement(base, base_cyclic, target_pose_increased, f"{arm_name}增加位姿"):
                print("增加位姿运动失败，程序退出")
                return 1
            
            # 获取增加位姿后的关节角度和位姿
            print("\n增加位姿后状态:")
            increased_angles, increased_position, increased_orientation, increased_feedback = get_joint_angles_and_pose(base, base_cyclic)
            print_joint_angles_and_pose(increased_angles, increased_position, increased_orientation, increased_feedback, "增加位姿后")
            
            # 停顿2秒
            print("\n停顿2秒...")
            time.sleep(2)
            
            # 计算减小位姿后的目标位姿（回到初始位姿附近）
            target_pose_decreased = {
                'x': increased_feedback.base.tool_pose_x - 0.02,
                'y': increased_feedback.base.tool_pose_y - 0.02,
                'z': increased_feedback.base.tool_pose_z - 0.02,
                'theta_x': increased_feedback.base.tool_pose_theta_x - 2.0,  # 减小2度
                'theta_y': increased_feedback.base.tool_pose_theta_y - 2.0,  # 减小2度
                'theta_z': increased_feedback.base.tool_pose_theta_z - 2.0   # 减小2度
            }
            
            # 执行减小位姿运动
            if not execute_cartesian_movement(base, base_cyclic, target_pose_decreased, f"{arm_name}减小位姿"):
                print("减小位姿运动失败，程序退出")
                return 1
            
            # 获取减小位姿后的关节角度和位姿
            print("\n减小位姿后状态:")
            final_angles, final_position, final_orientation, final_feedback = get_joint_angles_and_pose(base, base_cyclic)
            print_joint_angles_and_pose(final_angles, final_position, final_orientation, final_feedback, "减小位姿后")
            
            print(f"\n{arm_name}笛卡尔空间运动控制程序执行完成！")
            return 0
            
    except Exception as e:
        print(f"程序执行出错: {e}")
        return 1

if __name__ == "__main__":
    exit(main())