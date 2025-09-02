cmake_minimum_required(VERSION 3.5)
project(serial)

find_package(ament_cmake REQUIRED)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

## ------------ 库 -------------
add_library(serial SHARED
  src/serial.cc
  include/serial/serial.h
  include/serial/v8stdint.h
)


if(APPLE)
  target_sources(serial PRIVATE
    src/impl/unix.cc
    src/impl/list_ports/list_ports_osx.cc)
  target_link_libraries(serial Foundation IOKit)
elseif(UNIX)
  target_sources(serial PRIVATE
    src/impl/unix.cc
    src/impl/list_ports/list_ports_linux.cc)
  target_link_libraries(serial pthread rt)
elseif(WIN32)
  target_sources(serial PRIVATE
    src/impl/win.cc
    src/impl/list_ports/list_ports_win.cc)
  target_link_libraries(serial setupapi)
endif()


target_include_directories(serial
  PUBLIC    
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

## ------------ 安装 & 导出 -------------
install(TARGETS serial
  EXPORT  export_serial
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(
  DIRECTORY include/       
  DESTINATION include)

# 导出
install(EXPORT export_serial
  DESTINATION share/${PROJECT_NAME}/cmake)

ament_export_targets(export_serial)
ament_export_include_directories(include)
ament_export_libraries(serial)

ament_package()
