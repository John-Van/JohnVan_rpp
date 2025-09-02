cmake_minimum_required(VERSION 3.8)
project(wheeltec_mic_ros2)

# 编译选项与调试宏
set(CMAKE_BUILD_TYPE Debug)
add_compile_definitions(WORKSPACE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 头文件路径
include_directories(include)

link_directories(lib/x64)

# 安装资源（如配置文件等）
install(DIRECTORY launch config feedback_voice
  DESTINATION share/${PROJECT_NAME})

# ────── 查找依赖 ──────
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(serial REQUIRED)
find_package(wheeltec_mic_msg REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# 让 JSONCPP 的头文件路径生效
include_directories(${JSONCPP_INCLUDE_DIRS})

# ────── 可执行程序定义与安装 ──────

add_executable(call_recognition src/call_recognition.cpp)
ament_target_dependencies(call_recognition rclcpp std_msgs wheeltec_mic_msg)
install(TARGETS call_recognition DESTINATION lib/${PROJECT_NAME})

add_executable(command_recognition src/command_recognition.cpp)
ament_target_dependencies(command_recognition rclcpp std_msgs)
install(TARGETS command_recognition DESTINATION lib/${PROJECT_NAME})

add_executable(node_feedback src/node_feedback.cpp)
ament_target_dependencies(node_feedback rclcpp std_msgs)
install(TARGETS node_feedback DESTINATION lib/${PROJECT_NAME})

add_executable(wheeltec_mic src/wheeltec_mic.cpp src/ros2_service.cpp)
ament_target_dependencies(wheeltec_mic rclcpp std_msgs serial wheeltec_mic_msg)
target_link_libraries(wheeltec_mic ${JSONCPP_LIBRARIES} serial)
install(TARGETS wheeltec_mic DESTINATION lib/${PROJECT_NAME})

add_executable(voice_control src/voice_control.cpp)
ament_target_dependencies(voice_control rclcpp std_msgs wheeltec_mic_msg)
target_link_libraries(voice_control offline_record_lib rt dl pthread stdc++ asound)
set_target_properties(voice_control PROPERTIES INSTALL_RPATH "$ORIGIN/../../lib")
install(TARGETS voice_control DESTINATION lib/${PROJECT_NAME})

# 如果你需要安装外部库
install(DIRECTORY lib/x64/ DESTINATION lib)

# ament 导出
ament_package()
