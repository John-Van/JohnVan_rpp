cmake_minimum_required(VERSION 3.8)
project(xf_voice_msgs)

# 编译器警告（可选）
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 依赖包
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)

# 接口生成
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/Xf.srv"
  DEPENDENCIES builtin_interfaces
)

# 导出依赖（用于其他包依赖此接口包）
ament_export_dependencies(rosidl_default_runtime)

# 测试工具（可选）
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_cpplint_FOUND TRUE)
  set(ament_cmake_copyright_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
