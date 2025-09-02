rpp_driver
=======================

## 简介

rpp_driver是史河机器人公司通用底盘的新版协议C++驱动库。目前支持的底盘有两轮差速、四轮差速、四轮四转、麦克纳姆轮等类型底盘。

## 协议

协议文档请参考[语雀文档](https://robotplusplus.yuque.com/rb25dr/comm/protocol-rs232-v2)

## 依赖

本项目为了保证多版本linux的兼容性，使用了Boost System库中的串口库。

- Boost System库

## 编译构建

1. 本项目使用CMake构建，请先安装CMake。
2. 本项目依赖于Boost System库，请先安装Boost System库,可以使用以下命令安装：

```
sudo apt-get install libboost-system-dev
```

2. 使用CMake构建项目，命令如下：

```
mkdir build
cd build
cmake ..
make
```

3. 也可以直接使用cmake的find_package命令来导入项目，命令如下：

```
find_package(rpp_driver REQUIRED)
```

**注意**： 由于本项目依赖于Boost System，在你的项目中的CMakeLists.txt中添加以下代码：

```
find_package(Boost REQUIRED COMPONENTS system)
```

## 使用

1. 核心类``class RPPDriver``，该类封装了串口通信、协议解析、数据发送、数据接收等功能，用户只需要调用该类中的方法即可实现底盘控制。该类的接口函数和协议文档一一对应，用户可以参考协议文档来使用该类。这里不做特别说明。

    另外，该类还提供了基于运动学的轮式里程计和车体运动速度估计信息。车体坐标系为右手坐标系，坐标中心定义为车轮连线几何中心，并非车体外形几何中心。

    **注意**：该类会自动创建一个串口通信线程，用户不需要手动创建线程。

2. 参数说明：
    - port: 串口设备名，例如：/dev/ttyUSB0
    - baud_rate: 波特率，本公司底盘默认出场波特率为230400
    - control_rate: 默认通信控制频率，单位：Hz
    - motion_model: 底盘类型，"2wd"：两轮差速；"4wd"：四轮差速；"4w4s"：四轮四转；"mecanum"：麦克纳姆轮；"ackermann"：阿克曼转向
    - wheel_radius: 车轮半径，单位：米
    - wheel_base: 车轮前后轴距，单位：米
    - wheel_bias: 车轮左右轮间距，单位：米
    - use_diff_twist: 车体运动速度估计使用车轮转速计算或者里程计微分计算。true：使用里程计微分计算;false：使用车轮转速计算。用户可以根据实际需求测试选择。
