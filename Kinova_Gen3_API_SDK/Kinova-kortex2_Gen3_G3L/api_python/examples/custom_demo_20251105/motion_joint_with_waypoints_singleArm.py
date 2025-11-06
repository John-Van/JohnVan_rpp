#!/usr/bin/env python3
"""
单机械臂关节运动控制程序
Kinova Gen3 机械臂关节慢速运动控制程序

功能描述：
1. 通过轨迹点(Waypoint)机制实现精确的慢速关节运动控制
2. 主要功能流程：
   - 可选移动到安全位置（需用户确认）
   - 读取当前关节角度
   - 减小所有关节5度（可选择慢速或正常速度）
   - 停留2秒
   - 恢复原始关节角度
   - 最终角度确认

技术实现：
1. 慢速运动控制：
   - 使用Base_pb2.AngularWaypoint设置目标角度
   - 通过duration参数强制指定运动时间（秒）
   - 启用use_optimal_blending实现平滑运动

2. 安全机制：
   - 运动前验证轨迹有效性(ValidateWaypointList)
   - 失败时自动回退到默认Action方式
   - 设置30秒超时保护

3. API调用：
   - ExecuteWaypointTrajectory：执行轨迹点运动
   - ExecuteAction：执行预定义动作
   - GetMeasuredJointAngles：获取当前关节角度

使用说明：
1. 运行时会提示选择运动方式：
   - 1: 慢速运动（推荐）
   - 2: 正常速度运动
2. 可自定义慢速运动的持续时间（修改duration_seconds参数）

作者：JohnVan with CodeBuddy
创建日期：2025-11-05
修改日期：2025-11-05    
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
from kortex_api.autogen.messages import Base_pb2
from kortex_api.autogen.messages import ActuatorConfig_pb2
from kortex_api.Exceptions.KServerException import KServerException

import utilities

# 运动超时时间（秒）
TIMEOUT_DURATION = 30

# 运动完成检测回调函数（参照官方示例）
def check_for_end_or_abort(e):
    """检查运动是否完成或中止"""
    def check(notification, e=e):
        print("EVENT : " + Base_pb2.ActionEvent.Name(notification.action_event))
        if notification.action_event == Base_pb2.ACTION_END or notification.action_event == Base_pb2.ACTION_ABORT:
            e.set()
    return check

def print_joint_angles(joint_angles, arm_name="机械臂"):
    """打印关节角度信息"""
    print(f"{arm_name}当前关节角度:")
    for i, angle in enumerate(joint_angles.joint_angles):
        print(f"  关节{i+1}: {angle.value:.2f}°")
    print()

def create_angular_action(base_client, target_angles, action_name="Joint Movement Action"):
    """创建关节角度运动Action（参照官方示例）"""
    
    print(f"创建运动Action: {action_name}")
    action = Base_pb2.Action()
    action.name = action_name
    action.application_data = ""
    
    # 设置目标关节角度
    for i, angle in enumerate(target_angles):
        joint_angle = action.reach_joint_angles.joint_angles.joint_angles.add()
        joint_angle.joint_identifier = i
        joint_angle.value = angle.value
    
    return action

def execute_action(base_client, action):
    """执行Action并等待完成（参照官方示例）"""
    
    try:
        # 确保在单级伺服模式下
        base_servo_mode = Base_pb2.ServoingModeInformation()
        base_servo_mode.servoing_mode = Base_pb2.SINGLE_LEVEL_SERVOING
        base_client.SetServoingMode(base_servo_mode)
        
        # 设置运动完成检测
        e = threading.Event()
        notification_handle = base_client.OnNotificationActionTopic(
            check_for_end_or_abort(e),
            Base_pb2.NotificationOptions()
        )
        
        print("执行Action...")
        # 使用ExecuteAction而不是PlayJointTrajectory，这样运动更平滑
        base_client.ExecuteAction(action)
        
        print("等待运动完成...")
        finished = e.wait(TIMEOUT_DURATION)
        base_client.Unsubscribe(notification_handle)
        
        if finished:
            print("运动完成!")
            return True
        else:
            print("运动超时!")
            return False
        
    except KServerException as ex:
        print(f"API错误: 错误代码={ex.get_error_code()}")
        return False
    except Exception as e:
        print(f"运动控制错误: {e}")
        return False

def execute_slow_movement(base_client, target_angles, action_name="慢速运动", duration_seconds = 5.0):
    """使用轨迹点执行慢速运动"""  # 强制机械臂用 duration_seconds 秒完成该段运动，相当于降低速度了，在此处改数没用，要在具体调用的步骤上修改
    
    try:
        print(f"执行{action_name}（预计用时{duration_seconds}秒）...")
        
        # 确保在单级伺服模式下
        base_servo_mode = Base_pb2.ServoingModeInformation()
        base_servo_mode.servoing_mode = Base_pb2.SINGLE_LEVEL_SERVOING
        base_client.SetServoingMode(base_servo_mode)
        
        # 创建轨迹点列表
        waypoints = Base_pb2.WaypointList()
        waypoints.duration = 0.0
        waypoints.use_optimal_blending = True  # 使用最优混合以获得更平滑的运动
        
        # 添加目标轨迹点
        waypoint = waypoints.waypoints.add()
        waypoint.name = action_name
        
        # 创建角度轨迹点
        angular_waypoint = Base_pb2.AngularWaypoint()
        for angle in target_angles:
            angular_waypoint.angles.append(angle.value)
        angular_waypoint.duration = duration_seconds  # 设置运动持续时间
        
        waypoint.angular_waypoint.CopyFrom(angular_waypoint)
        
        # 验证轨迹有效性
        result = base_client.ValidateWaypointList(waypoints)
        if len(result.trajectory_error_report.trajectory_error_elements) > 0:
            print("轨迹验证失败，使用默认Action方式")
            action = create_angular_action(base_client, target_angles, action_name)
            return execute_action(base_client, action)
        
        # 设置运动完成检测
        e = threading.Event()
        notification_handle = base_client.OnNotificationActionTopic(
            check_for_end_or_abort(e),
            Base_pb2.NotificationOptions()
        )
        
        # 执行轨迹
        base_client.ExecuteWaypointTrajectory(waypoints)
        
        print("等待慢速运动完成...")
        finished = e.wait(TIMEOUT_DURATION)
        base_client.Unsubscribe(notification_handle)
        
        if finished:
            print("慢速运动完成!")
            return True
        else:
            print("慢速运动超时!")
            return False
        
    except Exception as e:
        print(f"慢速运动控制错误: {e}")
        # 如果轨迹方式失败，回退到Action方式
        action = create_angular_action(base_client, target_angles, action_name)
        return execute_action(base_client, action)

def move_to_position(base_client, ask_user=True):
    """移动到指定位置（参照官方示例）"""
    
    if ask_user:
        user_input = input("是否需要移动到指定安全位置？(y/n，默认n): ").strip().lower()
        if user_input not in ['y', 'yes', '是']:
            print("跳过移动到指定位置")
            return True
    
    print("移动到安全位置...")
    
    # 确保在单级伺服模式下
    base_servo_mode = Base_pb2.ServoingModeInformation()
    base_servo_mode.servoing_mode = Base_pb2.SINGLE_LEVEL_SERVOING
    base_client.SetServoingMode(base_servo_mode)
    
    # 查找Home动作
    action_type = Base_pb2.RequestedActionType()
    action_type.action_type = Base_pb2.REACH_JOINT_ANGLES
    action_list = base_client.ReadAllActions(action_type)
    action_handle = None
    
    for action in action_list.action_list:
        if action.name == "Left_Initial":  # 移动到指定的位姿
            action_handle = action.handle
            break
    
    if action_handle is None:
        print("未找到指定的位置，跳过...")
        return True
    
    try:
        e = threading.Event()
        notification_handle = base_client.OnNotificationActionTopic(
            check_for_end_or_abort(e),
            Base_pb2.NotificationOptions()
        )
        
        base_client.ExecuteActionFromReference(action_handle)
        finished = e.wait(TIMEOUT_DURATION)
        base_client.Unsubscribe(notification_handle)
        
        if finished:
            print("✓ 已到达指定安全位置")
            return True
        else:
            print("✗ 移动到指定安全位置超时")
            return False
            
    except Exception as e:
        print(f"移动到指定安全位置失败: {e}")
        return False

def main():
    """主程序"""
    
    # 机械臂配置
    arm_config = {
        "name": "左臂",
        "ip": "192.168.11.60", 
        "username": "admin",
        "password": "admin"
    }
    
    print(f"正在连接到{arm_config['name']}...")
    
    try:
        # 创建连接参数
        args = utilities.parseConnectionArguments()
        args.ip = arm_config["ip"]
        args.username = arm_config["username"]
        args.password = arm_config["password"]
        
        # 建立TCP连接
        connection = utilities.DeviceConnection.createTcpConnection(args)
        
        print(f"✓ {arm_config['name']} ({arm_config['ip']}) 连接成功")
        print()
        
        # 使用连接
        with connection as router:
            base_client = BaseClient(router)
            
            # 0. 移动到安全位置（可选）
            print("=== 步骤0: 移动到安全位置 ===")
            move_to_position(base_client, ask_user=True)
            print()
            
            # 1. 读取当前关节角度
            print("=== 步骤1: 读取当前关节角度 ===")
            current_angles = base_client.GetMeasuredJointAngles()
            print_joint_angles(current_angles, arm_config["name"])
            
            # 询问用户运动方式
            print("请选择运动方式:")
            print("1. 慢速运动（推荐，10秒完成）")
            print("2. 正常速度运动")
            choice = input("请输入选择 (1/2，默认1): ").strip()
            use_slow_movement = choice != '2'
            
            # 2. 计算目标角度（减小5度）
            print("\n=== 步骤2: 移动到减小5度的位置 ===")
            target_angles_minus_5 = Base_pb2.JointAngles()
            for angle in current_angles.joint_angles:
                new_angle = target_angles_minus_5.joint_angles.add()
                new_angle.value = angle.value - 5.0  # 减小5度
            
            # 执行减小5度的运动
            if use_slow_movement:
                success = execute_slow_movement(base_client, target_angles_minus_5.joint_angles, "减小5度位置", 5.0)
            else:
                action_minus_5 = create_angular_action(base_client, target_angles_minus_5.joint_angles, "减小5度位置")
                success = execute_action(base_client, action_minus_5)
            
            if success:
                # 3. 停留2秒
                print("\n=== 步骤3: 停留2秒 ===")
                print("保持当前位置...")
                time.sleep(2)
                print("停留完成")
                
                # 4. 恢复原位置
                print("\n=== 步骤4: 恢复到原位置 ===")
                if use_slow_movement:
                    success = execute_slow_movement(base_client, current_angles.joint_angles, "恢复原位置", 5.0)
                else:
                    action_restore = create_angular_action(base_client, current_angles.joint_angles, "恢复原位置")
                    success = execute_action(base_client, action_restore)
                
                if success:
                    print("✓ 运动序列完成!")
                else:
                    print("✗ 恢复原位置失败")
            else:
                print("✗ 移动到减小5度位置失败")
            
            # 5. 最终读取关节角度确认
            print("\n=== 步骤5: 最终关节角度确认 ===")
            final_angles = base_client.GetMeasuredJointAngles()
            print_joint_angles(final_angles, arm_config["name"])
            
    except Exception as e:
        print(f"✗ 连接失败: {e}")
        return

if __name__ == "__main__":
    main()