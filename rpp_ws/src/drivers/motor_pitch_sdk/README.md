# 钛虎电机驱动 SDK

对应 EA200 的俯仰电机，实现位置控制、速度控制、急停、设置零点。

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
mkdir build
cd build/
cmake ..
make
./rpp_keyboard_teleoperate
```

### 零位说明

默认以竖直状态为零位，俯向为正，行程为0° ~ 90°.
