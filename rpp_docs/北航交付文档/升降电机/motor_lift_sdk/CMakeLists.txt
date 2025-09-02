cmake_minimum_required(VERSION 3.5)
project(motor_lift_sdk)
set(version 0.0.1)
# 设置构建类型为 Debug，自动添加 -g 选项
set(CMAKE_BUILD_TYPE Debug)

# 添加头文件搜索路径
include_directories(
  include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_library(${PROJECT_NAME} SHARED
    src/motor_lift.cpp
    src/can_motor.cpp
)

target_link_libraries(${PROJECT_NAME}
)

add_executable(rpp_keyboard_teleoperate
    examples/keyboard_teleoperate.cpp
    src/can_motor.cpp
    src/motor_lift.cpp
)

target_link_libraries(rpp_keyboard_teleoperate
    ${PROJECT_NAME}
    ${LIB_SERIALPORT_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} rpp_keyboard_teleoperate
    EXPORT ${PROJECT_NAME}Config
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(
    DIRECTORY include/
    DESTINATION include
)
install(EXPORT ${PROJECT_NAME}Config
        DESTINATION share/${PROJECT_NAME}/cmake)




