cmake_minimum_required(VERSION 3.8)
project(xf_voice LANGUAGES C CXX)

########################################################
# 1. 编译选项
########################################################
add_compile_options(-Wall -Wextra -Wpedantic -std=c++17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
########################################################
# 2. 依赖
########################################################
find_package(ament_cmake REQUIRED)
find_package(rclcpp       REQUIRED)
find_package(std_msgs     REQUIRED)
find_package(xf_voice_msgs     REQUIRED)
# 查找 libmsc.so
find_library(MSC_LIB
  NAMES msc                      
  PATHS  ${CMAKE_CURRENT_SOURCE_DIR}/libs/x64
  NO_DEFAULT_PATH)

if(NOT MSC_LIB)
  message(FATAL_ERROR "libmsc.so not found!")
endif()

########################################################
# 3. 头文件、库文件路径
########################################################
# 讯飞 SDK 头文件
include_directories(include)


link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/x64)

########################################################

########################################################
add_executable(iat_publish
  src/iat_online_record_sample/iat_online_record_node.cpp
  src/iat_online_record_sample/speech_recognizer.c
  src/iat_online_record_sample/linuxrec.c
)

target_link_libraries(iat_publish
  rclcpp::rclcpp
  ${std_msgs_TARGETS}
  ${MSC_LIB}             # libmsc.so （由 link_directories 找到）
  dl pthread m rt asound
)


########################################################

########################################################
add_executable(tts_subscribe
  src/tts_online_sample/tts_online_node.cpp
)
ament_target_dependencies(tts_subscribe
  rclcpp
  std_msgs
  xf_voice_msgs      
)


target_link_libraries(tts_subscribe
  rclcpp::rclcpp
  ${std_msgs_TARGETS}
  ${MSC_LIB}
  dl pthread          
)

########################################################
# 6. 安装规则
########################################################
# 6.1 安装可执行文件
install(TARGETS
  iat_publish
  tts_subscribe
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)


install(
  DIRECTORY libs/x64/
  DESTINATION lib
)


install(
  DIRECTORY include/
  DESTINATION include
)

install(DIRECTORY launch/ config/
  DESTINATION share/${PROJECT_NAME}/)

  install(
  FILES
    src/voice_control/__init__.py
    src/voice_control/match_demo.py
    src/voice_control/robot_state.py
  DESTINATION lib/${PROJECT_NAME}
)
install(PROGRAMS
  src/voice_control/voice_control.py
  DESTINATION lib/${PROJECT_NAME}
  RENAME voice_control
)


# ---------------------- 新增：安装 voice_dialog_node.py 节点 ----------------------
# 1. 若 voice_dialog_node.py 依赖其他 Python 模块（如无则可省略），需安装 __init__.py（确保模块识别）
# （若 src/xf_voice/ 目录下没有 __init__.py，先手动创建一个空文件，再添加以下安装规则）
install(
  FILES
    src/xf_voice/__init__.py  # 空文件即可，用于标记 Python 模块目录
  DESTINATION lib/${PROJECT_NAME}
)

# 2. 安装 voice_dialog_node.py 为可执行程序，并重命名为 "voice_dialog"（方便启动）
install(PROGRAMS
  src/xf_voice/voice_dialog_node.py  # 你的新节点脚本路径
  DESTINATION lib/${PROJECT_NAME}    # 安装到与 voice_control 相同的目录（lib/xf_voice/）
  RENAME voice_dialog                # 启动命令简化为 "ros2 run xf_voice voice_dialog"
)
# --------------------------------------------------------------------------------
########################################################
# 7. 导出 package
########################################################
ament_package()
