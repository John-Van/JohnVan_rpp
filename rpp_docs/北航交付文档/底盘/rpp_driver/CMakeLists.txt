cmake_minimum_required(VERSION 3.5)
project(rpp_driver)
set(version 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

include_directories(
    include
    ${Boost_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} SHARED
    src/ring_buffer.cpp
    src/command.cpp
    src/rpp_driver.cpp
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${Boost_LIBRARIES}
        pthread
)

add_executable(rpp_keyboard_teleoperate
    examples/keyboard_teleoperate.cpp
)

target_include_directories(rpp_driver
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
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
