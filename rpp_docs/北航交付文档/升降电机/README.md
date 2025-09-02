# 升降电机驱动包

本目录包含针对 EA200 升降电机的两个驱动包，提供完整的升降控制解决方案。

## 包含组件

### 1. motor_lift_sdk
基础 SDK 包，提供 C++ 原生接口用于控制和利时电机。

**主要功能：**
- 位置控制（-46 cm ~ 0 cm 行程范围）
- 速度控制
- 急停功能
- 零点设置
- CAN 通信接口

**特点：**
- 独立的 C++ 库，可直接集成到任何项目中
- 提供键盘控制示例程序
- 基于 CMake 构建系统
- 位置单位：厘米（cm）

### 2. motor_lift_ros
ROS2 驱动包，基于 motor_lift_sdk 构建，提供标准 ROS2 接口。

**主要功能：**
- 发布电机位置信息
- 接收位置/速度设置指令
- 提供使能/失能服务
- 提供急停服务
- 移动模式控制

**ROS2 接口：**

#### 话题 (Topics)
- `/motor_lift/position` - 发布当前位置（米制，-0.46~0 m，向下为负方向）
- `/motor_lift/set_position` - 设置目标位置
- `/motor_lift/set_velocity` - 设置速度（0-0.04 m/s）
- `/motor_lift/set_move` - 移动模式控制

#### 服务 (Services)
- `/motor_lift/set_enable` - 电机使能控制
- `/motor_lift/set_emergency_stop` - 急停控制

## 硬件要求

- EA200 升降电机
- 和利时电机控制器
- Canable 设备（CAN-USB 转换器）
- 光电限位开关
- 支持 CAN 通信的计算机

## 环境依赖

### 系统依赖
```bash
sudo apt install can-utils
sudo apt-get install net-tools
```

### ROS2 依赖（仅用于 motor_lift_ros）
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
cd motor_lift_sdk
mkdir build && cd build
cmake ..
make
./rpp_keyboard_teleoperate  # 运行键盘控制示例
```

### 3. 使用 ROS2 包

```bash
# 编译
colcon build --packages-select motor_lift_ros

# 运行
source install/setup.bash
ros2 launch motor_lift_ros motor_lift_ros.launch.py
```

#### ROS2 控制示例
```bash
# 设置位置到中间位置（-0.23 米）
ros2 topic pub /motor_lift/set_position std_msgs/msg/Float32 "data: -0.23"

# 设置移动速度为 0.02 m/s
ros2 topic pub /motor_lift/set_velocity std_msgs/msg/Float32 "data: 0.02"

# 向上移动（大于0.5）
ros2 topic pub /motor_lift/set_move std_msgs/msg/Float32 "data: 1.0"

# 向下移动（小于0.5）
ros2 topic pub /motor_lift/set_move std_msgs/msg/Float32 "data: 0.3"

# 停止移动
ros2 topic pub /motor_lift/set_move std_msgs/msg/Float32 "data: 0.0"

# 急停
ros2 service call /motor_lift/set_emergency_stop std_srvs/srv/SetBool "data: true"

# 重新使能
ros2 service call /motor_lift/set_enable std_srvs/srv/SetBool "data: true"
```

## 坐标系说明

### 统一坐标系定义
ROS2 包直接调用 SDK，因此使用相同的坐标系定义，仅单位不同：

- **零点位置**：上光电限位下 1cm 处
- **坐标方向**：向下为负方向
- **行程范围**：-46 cm ~ 0 cm（SDK）/ -0.46 m ~ 0 m（ROS2）
- **单位转换**：ROS2 位置值 = SDK 位置值 / 100（厘米转米）

## 安全注意事项

1. **限位保护**：系统配备上下光电限位开关，防止超程
2. **急停功能**：任何时候都可以通过急停服务立即停止电机
3. **使能控制**：急停后必须重新使能才能继续操作
4. **速度限制**：
   - SDK：建议速度不超过 4 cm/s
   - ROS2：最大速度 0.04 m/s
5. **负载限制**：请参考升降电机手册中的负载规格

## 目录结构

```
升降电机/
├── README.md                    # 本文档
├── 升降电机手册.pdf              # 电机详细技术手册
├── motor_lift_sdk/              # 基础 SDK 包
│   ├── CMakeLists.txt
│   ├── package.xml
│   ├── README.md
│   ├── examples/                # 示例程序
│   ├── include/                 # 头文件
│   └── src/                     # 源代码
└── motor_lift_ros/              # ROS2 驱动包
    ├── CMakeLists.txt
    ├── package.xml
    ├── README.md
    ├── config/                  # 配置文件
    ├── include/                 # 头文件
    ├── launch/                  # 启动文件
    └── src/                     # 源代码
```

## 故障排除

### 常见问题

1. **设备无法识别**
   - 检查 USB 连接
   - 确认 Canable 设备驱动正确安装
   - 检查设备权限：`sudo chmod 666 /dev/ttyACM*`

2. **CAN 通信失败**
   - 确认波特率设置正确（-s8 对应 1Mbps）
   - 检查 CAN 总线接线
   - 确认电机控制器电源正常

3. **电机无响应**
   - 检查电机电源连接
   - 确认 CAN 总线接线正确
   - 检查限位开关状态

4. **位置读取异常**
   - 检查编码器连接
   - 确认零点设置正确
   - 重新校准限位位置

5. **急停后无法控制**
   - 急停后需要重新调用使能服务
   - 检查限位开关是否触发

### 调试命令

```bash
# 检查 CAN 接口状态
ifconfig can0

# 监听 CAN 总线数据
candump can0

# 检查 ROS2 话题
ros2 topic list | grep motor_lift
ros2 topic echo /motor_lift/position

# 检查 ROS2 服务
ros2 service list | grep motor_lift
```

## 性能参数

- **行程范围**：46 cm
- **最大速度**：0.04 m/s (4 cm/s)
- **位置精度**：±1 mm
- **重复定位精度**：±0.5 mm
- **最大负载**：请参考升降电机手册

## 技术支持

详细的电机技术参数和维护说明请参考 `升降电机手册.pdf` 文档。

各个包的具体使用方法请参考对应目录下的 README.md 文件。
