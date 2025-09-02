cmake_minimum_required(VERSION 3.5)
project(lidar_clipping)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(livox_ros_driver2 REQUIRED)


add_executable(lidar_clipping src/lidar_clipping.cpp)

ament_target_dependencies(lidar_clipping
  rclcpp
  sensor_msgs
  livox_ros_driver2
)
install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME})
install(TARGETS
  lidar_clipping
  DESTINATION lib/${PROJECT_NAME}
)


ament_package()
