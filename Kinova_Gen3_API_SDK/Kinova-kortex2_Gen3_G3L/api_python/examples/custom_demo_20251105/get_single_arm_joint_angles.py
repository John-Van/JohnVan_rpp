#!/usr/bin/env python3
# 机械臂关节转角状态读取程序
# 功能：专门获取当前机械臂的关节角度信息

import sys
import os
from kortex_api.autogen.client_stubs.BaseClientRpc import BaseClient
from kortex_api.Exceptions.KServerException import KServerException

# 导入examples中的工具模块
sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import utilities

def get_joint_angles():
    # 创建连接参数对象
    args = utilities.parseConnectionArguments()
    
    # 硬编码机械臂连接参数
    args.ip = "192.168.11.60"  # 60为左臂，61为右臂
    args.username = "admin"
    args.password = "admin"

    try:
        # 建立TCP连接（示例标准连接方式）
        with utilities.DeviceConnection.createTcpConnection(args) as router:
            # 初始化基础服务客户端
            base = BaseClient(router)

            # 获取关节角度
            print("===== 机械臂关节角度（度） =====")
            joint_angles = base.GetMeasuredJointAngles()
            print("关节ID | 角度值")
            print("----------------")
            for joint in joint_angles.joint_angles:
                print(f"   {joint.joint_identifier:<4} | {joint.value:.2f}")

            return True

    except KServerException as ex:
        print(f"\nAPI错误: 错误代码={ex.get_error_code()}, 子代码={ex.get_error_sub_code()}")
        print(f"错误信息: {ex}")
        return False
    except Exception as e:
        print(f"\n未知错误: {e}")
        return False

if __name__ == "__main__":
    success = get_joint_angles()
    sys.exit(0 if success else 1)