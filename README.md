# Kinova_DualArm

Kinova_DualArm 是一个用于控制 Kinova 双臂机器人（如 Gen3/Gen3 Lite）的 ROS2 驱动与应用示例仓库。该项目旨在提供机器人控制、视觉感知、运动规划等功能的基础设施，支持在真实硬件与仿真环境中运行。

## 📦 项目结构
Kinova_DualArm/
├── rpp_data/ # 机器人外设数据
├── rpp_docs/ # 技术文档与说明
├── rpp_logs/ # 日志文件
├── rpp_tools/ # 工具脚本
├── rpp_ws/ # ROS2 工作空间
├── README.md # 项目说明文件

## 🔧 主要功能

- **机械臂控制**：支持 Kinova Gen3/Gen3 Lite 系列机械臂的控制，包括关节控制、笛卡尔控制和夹爪控制。
- **仿真支持**：提供与 Gazebo 和 V-REP 的集成，支持在仿真环境中进行开发和测试。
- **视觉感知**：集成 Kinova Vision 模块，支持图像流和点云数据处理。
- **运动规划**：与 MoveIt! 集成，实现路径规划和运动控制。

## 🛠 安装与配置

### 1. 环境要求

- **操作系统**：Ubuntu 18.04 或 20.04
- **ROS2 版本**：建议使用 ROS2 Foxy 或 Galactic
- **硬件要求**：Kinova Gen3 或 Gen3 Lite 机械臂；可选：Kinova Vision 模块

### 2. 安装步骤

1. **克隆本仓库**：
   ```bash
   git clone https://github.com/John-Van/Kinova_DualArm.git
   cd Kinova_DualArm

2. **创建 ROS2 工作空间并构建**：
   colcon build --symlink-install
   source install/setup.bash

3. **安装依赖**：
   sudo apt-get install ros-<ros2-distro>-moveit
   请将 <ros2-distro> 替换为您使用的 ROS2 版本，如 foxy 或 galactic。

4. **配置环境变量**：
   echo "source ~/Kinova_DualArm/install/setup.bash" >> ~/.bashrc
   source ~/.bashrc

## 🚀 使用指南

1. **启动仿真环境**：
   bash
   复制
   编辑
   ros2 launch rpp_tools bringup.launch.py
   此命令将启动仿真环境，并加载相应的场景和控制节点。

2. **控制机械臂**：
    bash
   复制
   编辑
   ros2 launch rpp_tools control.launch.py
   该命令将启动控制节点，允许用户通过 ROS2 接口控制机械臂的运动。

## 📄 文档与支持
    技术文档：详见 rpp_docs/ 目录，包含各模块的使用说明和配置指南。

    示例代码：rpp_tools/ 目录下提供了多个示例脚本，帮助用户快速上手。

    常见问题：如遇问题，请查阅 rpp_logs/ 目录下的日志文件，或在 GitHub Issues 中提交您的问题。
