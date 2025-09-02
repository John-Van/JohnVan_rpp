# 俯仰电机驱动包

本目录包含针对 EA200 俯仰电机的两个驱动包，提供完整的电机控制解决方案。

## 包含组件

### 1. motor_pitch_sdk
基础 SDK 包，提供 C++ 原生接口用于控制钛虎电机。

**主要功能：**
- 位置控制（0° ~ 90° 行程范围）
- 速度控制
- 急停功能
- 零点设置
- CAN 通信接口

**特点：**
- 独立的 C++ 库，可直接集成到任何项目中
- 提供键盘控制示例程序
- 基于 CMake 构建系统

### 2. motor_pitch_ros
ROS2 驱动包，基于 motor_pitch_sdk 构建，提供标准 ROS2 接口。

**主要功能：**
- 发布电机位置信息
- 接收位置/速度设置指令
- 提供使能/失能服务
- 提供急停服务
- 移动模式控制

**ROS2 接口：**

#### 话题 (Topics)
- `/motor_pitch/position` - 发布当前位置（弧度制，0~1.5708 对应 0~90°）
- `/motor_pitch/set_position` - 设置目标位置
- `/motor_pitch/set_velocity` - 设置速度（0-10 RPM）
- `/motor_pitch/set_move` - 移动模式控制

#### 服务 (Services)
- `/motor_pitch/set_enable` - 电机使能控制
- `/motor_pitch/set_emergency_stop` - 急停控制

## 硬件要求

- EA200 俯仰电机
- Canable 设备（CAN-USB 转换器）
- 支持 CAN 通信的计算机

## 环境依赖

### 系统依赖
```bash
sudo apt install can-utils
sudo apt-get install net-tools
```

### ROS2 依赖（仅用于 motor_pitch_ros）
- ROS2 Humble/Foxy 或更高版本
- std_msgs
- std_srvs

## 快速开始

### 1. 硬件连接和设备配置

#### 查找 Canable 设备
```bash
ls /dev/tty*
```

#### 绑定设备并启动 CAN 通信
```bash
sudo slcand -o -c -s8 /dev/ttyACM0 can0  # 根据实际端口号修改
sudo ifconfig can0 up
```

### 2. 使用 SDK（C++ 原生开发）

```bash
cd motor_pitch_sdk
mkdir build && cd build
cmake ..
make
./rpp_keyboard_teleoperate  # 运行键盘控制示例
```

### 3. 使用 ROS2 包

```bash
# 编译
colcon build --packages-select motor_pitch_ros

# 运行
source install/setup.bash
ros2 launch motor_pitch_ros motor_pitch_ros.launch.py
```

#### ROS2 控制示例
```bash
# 设置位置到 45 度（0.7854 弧度）
ros2 topic pub /motor_pitch/set_position std_msgs/msg/Float32 "data: 0.7854"

# 设置速度为 5 RPM
ros2 topic pub /motor_pitch/set_velocity std_msgs/msg/Float32 "data: 5.0"

# 急停
ros2 service call /motor_pitch/set_emergency_stop std_srvs/srv/SetBool "data: true"

# 重新使能
ros2 service call /motor_pitch/set_enable std_srvs/srv/SetBool "data: true"
```

## 坐标系说明

- **零点位置**：电机竖直状态
- **正方向**：俯向为正
- **行程范围**：0° ~ 90°（0 ~ 1.5708 弧度）

## 目录结构

```
俯仰电机/
├── README.md                    # 本文档
├── 俯仰电机协议.xlsx            # 电机通信协议文档
├── motor_pitch_sdk/             # 基础 SDK 包
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── examples/                # 示例程序
│   ├── include/                 # 头文件
│   └── src/                     # 源代码
└── motor_pitch_ros/             # ROS2 驱动包
    ├── CMakeLists.txt
    ├── package.xml
    ├── README.md
    ├── config/                  # 配置文件
    ├── include/                 # 头文件
    ├── launch/                  # 启动文件
    └── src/                     # 源代码
```

## 故障排除

1. **设备无法识别**：检查 USB 连接，确认 Canable 设备驱动正确安装
2. **CAN 通信失败**：确认波特率设置正确（-s8 对应 1Mbps）
3. **电机无响应**：检查电机电源连接和 CAN 总线接线
4. **急停后无法控制**：急停后需要重新调用使能服务

## 技术支持

详细的协议说明请参考 `俯仰电机协议.xlsx` 文档。

各个包的具体使用方法请参考对应目录下的 README.md 文件。
