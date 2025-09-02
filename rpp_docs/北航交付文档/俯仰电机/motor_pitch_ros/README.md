# 钛虎电机 ROS 驱动包

对应 EA200 的俯仰电机，实现位置控制、速度控制、急停、设置零点。

依赖 SDK 包运行。

## 使用

### 环境配置

#### 安装依赖包：

```
sudo apt install can-utils
sudo apt-get install net-tools
```

#### 查找并绑定 Canable 设备

```
ls /dev/tty*
sudo slcand -o -c -s8 /dev/ttyACM0 can0
```

此处改为识别到的端口号，如/dev/ttyACM0

#### 启动 can 通信

```
sudo ifconfig can0 up
```

### 编译运行

```
colcon build
source install/setup.bash
ros2 launch motor_pitch_ros motor_pitch_ros.launch.py
```

## ROS2 话题：

/motor_pitch/position

俯仰电机的位置发布，单位 弧度

类型：std_msgs/msg/Float32

范围：0~1.5708，弧度制，对应0~90°，俯向为正方向，零点位置为竖直

/motor_pitch/set_position

设置俯仰电机的位置，其余同上

/motor_pitch/set_velocity

设置俯仰电机的速度，单位： RPM

类型：std_msgs/msg/Float32

范围：0-10

/motor_pitch/set_move

移动模式旋转俯仰电机

类型：std_msgs/msg/Float32

范围：任意，大于 0.5 则俯向旋转5度，小于 -0.5 则抬起5度，其余则停止

## ROS2 服务：

/motor_pitch/set_enable

俯仰电机使能，程序启动后默认使能

类型：std_srvs::srv::SetBool

ture:使能

false:失能

/motor_pitch/set_emergency_stop

俯仰电机急停

类型：std_srvs::srv::SetBool

ture:急停，急停后需重新使能方可操作

false:无操作
