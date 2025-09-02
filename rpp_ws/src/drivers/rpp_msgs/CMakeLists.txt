cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(rpp_msgs)

# Compile as C++11, supported in ROS Kinetic and newer

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

find_package(std_msgs REQUIRED)

# Add message files
set(msg_files
  "msg/BatteryState.msg"
  "msg/DeviceState.msg"
  "msg/ErrorState.msg"
  "msg/LightState.msg"
  "msg/MotorState.msg"
  "msg/SingleMotorState.msg"
  "msg/SetDevice.msg"
  "msg/SetErrorMask.msg"
  "msg/SetLight.msg"
)

# Generate messages and services
rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  DEPENDENCIES builtin_interfaces std_msgs
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()
