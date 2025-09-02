
#!/usr/bin/env python3
# -*- coding:utf-8 -*-
from enum import Enum
from enum import Enum, unique

@unique
class RobotState(Enum):
    # ==== 系统状态 ====
    IDLE = "空闲"            # 空闲状态
    BUSY = "忙碌"            # 正在执行任务
    IAT = "语音听写"         # 正在语音识别
    IAT_WAIT = "等待语音听写" # 等待语音输入

    # ==== 控制命令 ====
    SAVE_POSE = 0x01         # 保存当前位置
    CLEAR_POSE = 0x02        # 清除所有位置
    NEXT_POSE = 0x03         # 跳转到下一个位置
    PREV_POSE = 0x04         # 跳转到上一个位置
    LOOK_AT_ME = 0x05        # 朝向我

    def is_status(self):
        """判断是否为状态类型"""
        return isinstance(self.value, str)

    def is_command(self):
        """判断是否为控制命令"""
        return isinstance(self.value, int)

    @classmethod
    def from_value(cls, value):
        """根据值查找对应的枚举成员"""
        for item in cls:
            if item.value == value:
                return item
        return None
