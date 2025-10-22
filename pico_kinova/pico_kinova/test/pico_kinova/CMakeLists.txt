cmake_minimum_required(VERSION 3.8)
project(pico_kinova)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dynamixel SDK configuration
set(DYNAMIXEL_SDK_DIR "${PROJECT_SOURCE_DIR}/dynamixel_sdk" CACHE PATH "Path to Dynamixel SDK")
set(DYNAMIXEL_SDK_INCLUDE_DIR "${DYNAMIXEL_SDK_DIR}/include")
set(DYNAMIXEL_SDK_LIB_DIR "${DYNAMIXEL_SDK_DIR}/lib")

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(rclcpp REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(moveit_msgs REQUIRED)
find_package(rcl_interfaces REQUIRED)
find_package(rosidl_runtime_c REQUIRED)
find_package(rosidl_typesupport_interface REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(moveit_core REQUIRED)
find_package(srdfdom REQUIRED) 
find_package(unique_identifier_msgs REQUIRED)

include_directories(${rclcpp_INCLUDE_DIRS})

# Find Eigen package first
find_package(Eigen3 QUIET)
if(NOT Eigen3_FOUND)
    # If regular find_package doesn't work, try with a custom directory
    set(EIGEN3_INCLUDE_DIR "C:/local/eigen-3.4.0")
    if(EXISTS "${EIGEN3_INCLUDE_DIR}")
        message(STATUS "Using Eigen from custom path: ${EIGEN3_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "Eigen library not found. Please install Eigen or specify the correct path.")
    endif()
else()
    message(STATUS "Found Eigen3: ${EIGEN3_INCLUDE_DIR}")
endif()

# ur_rtde configuration
set(PYTHON_BINDINGS OFF CACHE BOOL "Build Python bindings")

# Add ur_rtde
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ur_rtde")
    add_subdirectory(ur_rtde)
else()
    message(FATAL_ERROR 
        "ur_rtde not found. Please clone the repository:\n"
        "git clone https://gitlab.com/sdurobotics/ur_rtde.git"
    )
endif()

# Main executable
add_executable(pico_kinova src/pico_kinova.cpp)

target_include_directories(pico_kinova
    PRIVATE
        ${PROJECT_SOURCE_DIR}/lib
        ${PROJECT_SOURCE_DIR}/ur_rtde/include
        ${EIGEN3_INCLUDE_DIR}
        ${DYNAMIXEL_SDK_INCLUDE_DIR}
        ${rclcpp_INCLUDE_DIRS}
        ${geometry_msgs_INCLUDE_DIRS}
        ${moveit_ros_planning_interface_INCLUDE_DIRS}
        ${moveit_msgs_INCLUDE_DIRS}
        ${rcl_interfaces_INCLUDE_DIRS}
        ${rosidl_runtime_c_INCLUDE_DIRS}
        ${rosidl_typesupport_interface_INCLUDE_DIRS}
        ${moveit_core_INCLUDE_DIRS}
        ${srdfdom_INCLUDE_DIRS}
        ${unique_identifier_msgs_INCLUDE_DIRS}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_directories(pico_kinova
    PRIVATE
        ${DYNAMIXEL_SDK_LIB_DIR}
        ${PROJECT_SOURCE_DIR}/lib
)

target_link_libraries(pico_kinova
    PXREARobotSDK
    rtde
    nlohmann_json::nlohmann_json
    dxl_x64_cpp
    # 使用 ament_target_dependencies 处理 ROS 相关依赖
)

target_compile_features(pico_kinova PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

ament_target_dependencies(
  pico_kinova
  "rclcpp"
  "geometry_msgs"
  "moveit_ros_planning_interface"
  "moveit_msgs"
  "rcl_interfaces"
  "rosidl_runtime_c"
  "rosidl_typesupport_interface"
  "moveit_core"
  "srdfdom"
  "unique_identifier_msgs"
)

if(WIN32)
    target_link_libraries(pico_kinova
        PXREARobotSDK.dll
        rtde
        nlohmann_json::nlohmann_json
        dxl_x64_cpp
    )
    
    # Copy DLLs to output directory
    add_custom_command(TARGET pico_kinova POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:rtde>
            ${DYNAMIXEL_SDK_LIB_DIR}/dxl_x64_cpp.dll
            $<TARGET_FILE_DIR:pico_kinova>
    )
else()
    target_link_libraries(pico_kinova
        PXREARobotSDK
        rtde
        nlohmann_json::nlohmann_json
        dxl_x64_cpp
    )
endif()

include(GNUInstallDirs)
install(TARGETS pico_kinova
    RUNTIME DESTINATION lib/${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
