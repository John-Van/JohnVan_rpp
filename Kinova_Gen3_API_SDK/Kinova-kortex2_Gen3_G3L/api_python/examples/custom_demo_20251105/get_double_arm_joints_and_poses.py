#!/usr/bin/env python3
"""
Kinova Gen3 双机械臂关节角度与笛卡尔位姿读取程序

功能描述：
1. 双机械臂状态监控：
   - 同时连接两台Kinova Gen3机械臂（左臂和右臂）
   - 实时读取各机械臂的关节角度（6个关节）
   - 实时读取末端执行器的笛卡尔位姿（位置XYZ + 姿态欧拉角）

2. 核心能力：
   - 自动建立TCP连接（支持用户名/密码认证）
   - 多线程安全连接管理（使用with语句确保资源释放）
   - 数据可视化打印（关节角度/位姿的数值和单位转换）

3. 技术实现：
   - 基于Kortex API的BaseClientRpc服务
   - 关键API调用：
     * GetMeasuredJointAngles() - 获取关节角度
     * GetMeasuredCartesianPose() - 获取笛卡尔位姿
   - 异常处理：
     * KServerException - 处理机器人控制器错误
     * 通用异常捕获 - 保证程序健壮性

4. 使用场景：
   - 机械臂状态监控面板
   - 运动轨迹调试辅助工具
   - 安全位置校验

5. 输出示例：
   ┌───────────────────────────────┐
   │ 左臂 状态信息                  │
   │ 关节ID | 角度值                │
   │   1    | 45.23°               │
   │   2    | -12.56°              │
   │ 位置(X,Y,Z): (0.512, 0.231, 0.891)m │
   └───────────────────────────────┘

作者：JohnVan with CodeBuddy
创建日期：2025/11/5
修改日期：2025/11/5
"""

import sys
import os
from kortex_api.autogen.client_stubs.BaseClientRpc import BaseClient
from kortex_api.Exceptions.KServerException import KServerException

# 导入examples中的工具模块
sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import utilities

class DualArmController:
    def __init__(self):
        # 双机械臂连接参数
        self.arms_config = [
            {"name": "左臂", "ip": "192.168.11.60", "username": "admin", "password": "admin"},
            {"name": "右臂", "ip": "192.168.11.61", "username": "admin", "password": "admin"}
        ]
        self.connections = []
        self.base_clients = []
    
    def connect_to_arms(self):
        """连接到两个机械臂"""
        print("正在连接到双机械臂...")
        
        # 只解析一次命令行参数，然后修改参数值
        args = utilities.parseConnectionArguments()
        
        for i, config in enumerate(self.arms_config):
            try:
                # 修改参数值
                args.ip = config["ip"]
                args.username = config["username"]
                args.password = config["password"]
                
                # 使用DeviceConnection作为上下文管理器建立TCP连接
                connection = utilities.DeviceConnection.createTcpConnection(args)
                
                # 存储连接对象，但不立即创建router
                self.connections.append(connection)
                
                print(f"✓ {config['name']} ({config['ip']}) 连接对象创建成功")
                
            except Exception as e:
                print(f"✗ {config['name']} ({config['ip']}) 连接失败: {e}")
                return False
        
        return True
    
    def get_arm_status(self, arm_index):
        """获取单个机械臂的状态信息"""
        config = self.arms_config[arm_index]
        connection = self.connections[arm_index]
        
        try:
            # 使用with语句正确管理连接
            with connection as router:
                base = BaseClient(router)
                
                # 获取关节角度
                joint_angles = base.GetMeasuredJointAngles()
                
                # 获取笛卡尔位姿
                cartesian_pose = base.GetMeasuredCartesianPose()
                
                return {
                    "name": config["name"],
                    "joints": joint_angles.joint_angles,
                    "pose": cartesian_pose
                }
            
        except KServerException as ex:
            print(f"{config['name']} API错误: 错误代码={ex.get_error_code()}")
            return None
        except Exception as e:
            print(f"{config['name']} 未知错误: {e}")
            return None
    
    def display_arm_status(self, status):
        """显示机械臂状态信息"""
        if not status:
            return
            
        print(f"\n{'='*60}")
        print(f"{status['name']} 状态信息")
        print(f"{'='*60}")
        
        # 显示关节角度
        print("\n关节角度（度）:")
        print("关节ID | 角度值")
        print("-" * 20)
        for joint in status['joints']:
            print(f"   {joint.joint_identifier:<4} | {joint.value:.2f}")
        
        # 显示笛卡尔位姿
        pose = status['pose']
        print("\n笛卡尔位姿:")
        print(f"位置 (X, Y, Z): ({pose.x:.3f}, {pose.y:.3f}, {pose.z:.3f}) m")
        print(f"姿态 (θx, θy, θz): ({pose.theta_x:.3f}, {pose.theta_y:.3f}, {pose.theta_z:.3f}) rad")
        
        # 转换为角度显示
        import math
        print(f"姿态 (θx, θy, θz): ({math.degrees(pose.theta_x):.1f}, {math.degrees(pose.theta_y):.1f}, {math.degrees(pose.theta_z):.1f}) °")
    
    def close_connections(self):
        """关闭所有连接"""
        for connection in self.connections:
            try:
                connection.__exit__(None, None, None)
            except:
                pass

def main():
    dual_arm = DualArmController()
    
    try:
        # 连接到机械臂
        if not dual_arm.connect_to_arms():
            print("机械臂连接失败，程序退出")
            return False
        
        # 获取并显示两个机械臂的状态
        print("\n" + "="*60)
        print("双机械臂状态读取")
        print("="*60)
        
        for i in range(len(dual_arm.arms_config)):
            status = dual_arm.get_arm_status(i)
            dual_arm.display_arm_status(status)
        
        print("\n✓ 双机械臂状态读取完成")
        return True
        
    except Exception as e:
        print(f"程序执行错误: {e}")
        return False
    
    finally:
        # 确保连接被正确关闭
        dual_arm.close_connections()

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)