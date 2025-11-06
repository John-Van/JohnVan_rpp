#!/usr/bin/env python3
"""
机械臂左臂5度运动控制程序
Kinova Gen3 左臂关节运动控制程序

功能描述：
1. 通过ExecuteAction动作接口控制机械臂左臂运动
2. 左臂各关节先增加5度，再减小5度
3. 运动前后打印关节位置和位姿信息
4. 两次运动中间停顿2秒

技术实现：
1. 使用Base_pb2.Action创建关节角度运动
2. 通过ExecuteAction执行预定义动作
3. 获取并打印关节角度和笛卡尔位姿

使用说明：
- 左臂IP: 192.168.11.60
- 账户: admin
- 密码: admin

作者：JohnVan with CodeBuddy
创建日期：2025-11-05
"""

import time
import sys
import os
import threading

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

# 左臂连接参数
LEFT_ARM_IP = "192.168.11.60"
USERNAME = "admin"
PASSWORD = "admin"

def check_for_end_or_abort(e):
    """检查运动是否完成或中止"""
    def check(notification, e=e):
        print("EVENT : " + Base_pb2.ActionEvent.Name(notification.action_event))
        if notification.action_event == Base_pb2.ACTION_END or notification.action_event == Base_pb2.ACTION_ABORT:
            e.set()
    return check

def print_joint_angles(joint_angles, arm_name="左臂"):
    """打印关节角度信息"""
    print(f"{arm_name}当前关节角度:")
    for i, angle in enumerate(joint_angles.joint_angles):
        print(f"  关节{i+1}: {angle.value:.2f}°")
    print()

def print_cartesian_pose(feedback, arm_name="左臂"):
    """打印笛卡尔位姿信息"""
    print(f"{arm_name}当前笛卡尔位姿:")
    print(f"  位置 X: {feedback.base.tool_pose_x:.3f} m")
    print(f"  位置 Y: {feedback.base.tool_pose_y:.3f} m")
    print(f"  位置 Z: {feedback.base.tool_pose_z:.3f} m")
    print(f"  姿态 θx: {feedback.base.tool_pose_theta_x:.2f}°")
    print(f"  姿态 θy: {feedback.base.tool_pose_theta_y:.2f}°")
    print(f"  姿态 θz: {feedback.base.tool_pose_theta_z:.2f}°")
    print()

def create_angular_action(base, target_angles, action_name="Joint Movement"):
    """创建关节角度运动Action"""
    
    print(f"创建运动Action: {action_name}")
    action = Base_pb2.Action()
    action.name = action_name
    action.application_data = ""
    
    # 设置目标关节角度
    for i, angle in enumerate(target_angles):
        joint_angle = action.reach_joint_angles.joint_angles.joint_angles.add()
        joint_angle.joint_identifier = i
        joint_angle.value = angle
    
    return action

def execute_angular_movement(base, base_cyclic, target_angles, movement_name):
    """执行关节角度运动"""
    
    print(f"开始执行 {movement_name}...")
    
    # 创建动作
    action = create_angular_action(base, target_angles, movement_name)
    
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

def move_to_initial_position(base):
    """移动到初始位置"""
    
    print("移动左臂到Left_Initial位姿...")
    
    # 确保在单级伺服模式下
    base_servo_mode = Base_pb2.ServoingModeInformation()
    base_servo_mode.servoing_mode = Base_pb2.SINGLE_LEVEL_SERVOING
    base.SetServoingMode(base_servo_mode)
    
    # 查找Left_Initial动作
    action_type = Base_pb2.RequestedActionType()
    action_type.action_type = Base_pb2.REACH_JOINT_ANGLES
    action_list = base.ReadAllActions(action_type)
    action_handle = None
    
    for action in action_list.action_list:
        if action.name == "Left_Initial":
            action_handle = action.handle
            break
    
    if action_handle is None:
        print("未找到Left_Initial动作，尝试查找Home动作作为备选")
        for action in action_list.action_list:
            if action.name == "Home":
                action_handle = action.handle
                print("使用Home动作作为备选")
                break
    
    if action_handle is None:
        print("未找到Left_Initial或Home动作，跳过初始位置移动")
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

def main():
    """主函数"""
    
    print("=== 机械臂左臂5度运动控制程序 ===")
    print(f"目标IP: {LEFT_ARM_IP}")
    print()
    
    # 创建连接参数
    args = utilities.parseConnectionArguments()
    args.ip = LEFT_ARM_IP
    args.username = USERNAME
    args.password = PASSWORD
    
    try:
        # 创建TCP连接
        with utilities.DeviceConnection.createTcpConnection(args) as router:
            
            # 创建服务客户端
            base = BaseClient(router)
            base_cyclic = BaseCyclicClient(router)
            
            print("成功连接到左臂机械臂")
            
            # 1. 运动前打印关节位置和位姿
            print("=== 运动前状态 ===")
            initial_joint_angles = base.GetMeasuredJointAngles()
            initial_feedback = base_cyclic.RefreshFeedback()
            
            print_joint_angles(initial_joint_angles)
            print_cartesian_pose(initial_feedback)
            
            # 2. 移动到安全位置
            if not move_to_initial_position(base):
                print("安全位置移动失败，继续执行...")
            
            # 获取当前位置
            current_joint_angles = base.GetMeasuredJointAngles()
            current_angles = [angle.value for angle in current_joint_angles.joint_angles]
            
            print("当前位置关节角度:")
            for i, angle in enumerate(current_angles):
                print(f"  关节{i+1}: {angle:.2f}°")
            print()
            
            # 3. 各关节增加5度
            print("=== 执行各关节增加5度运动 ===")
            increased_angles = [angle + 5.0 for angle in current_angles]
            
            success = execute_angular_movement(base, base_cyclic, increased_angles, "各关节增加5度")
            
            if success:
                # 运动后打印状态
                print("=== 增加5度后状态 ===")
                after_increase_joint_angles = base.GetMeasuredJointAngles()
                after_increase_feedback = base_cyclic.RefreshFeedback()
                
                print_joint_angles(after_increase_joint_angles)
                print_cartesian_pose(after_increase_feedback)
                
                # 停顿2秒
                print("停顿2秒...")
                time.sleep(2)
                
                # 4. 各关节减小5度（回到原始位置）
                print("=== 执行各关节减小5度运动 ===")
                success = execute_angular_movement(base, base_cyclic, current_angles, "各关节减小5度")
                
                if success:
                    # 运动结束后打印状态
                    print("=== 运动结束后状态 ===")
                    final_joint_angles = base.GetMeasuredJointAngles()
                    final_feedback = base_cyclic.RefreshFeedback()
                    
                    print_joint_angles(final_joint_angles)
                    print_cartesian_pose(final_feedback)
                    
                    print("=== 运动完成 ===")
                    print("左臂成功完成各关节增加5度再减小5度的运动")
                else:
                    print("减小5度运动失败")
            else:
                print("增加5度运动失败")
            
            return 0 if success else 1
            
    except Exception as e:
        print(f"程序执行出错: {e}")
        return 1

if __name__ == "__main__":
    exit(main())