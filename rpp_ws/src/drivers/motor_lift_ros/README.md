# 和利时电机 ROS 驱动包

对应 EA200 的升降电机，实现位置控制、速度控制、急停、设置零点。

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
ros2 launch motor_lift_ros motor_lift_ros.launch.py
```

## ROS2 话题：

/motor_lift/position

升降电机的位置发布，单位 m

类型：std_msgs/msg/Float32

范围：-0.46~0，向上为正方向，零点位置为最上端

/motor_lift/set_position

设置升降电机的位置，其余同上

/motor_lift/set_velocity

设置升降电机的速度，单位：m/s

类型：std_msgs/msg/Float32

范围：0-0.04

/motor_lift/set_move

移动升降电机

类型：std_msgs/msg/Float32

范围：任意，大于 0 则向上移动 0.5cm，小于 0 则向下移动 0.5cm，0 则停

## ROS2 服务：

/motor_lift/set_enable

升降电机使能，程序启动后默认使能

类型：std_srvs::srv::SetBool

ture:使能

false:失能
/motor_lift/set_emergency_stop

升降电机急停

类型：std_srvs::srv::SetBool

ture:急停，急停后需重新使能方可操作

false:无操作
