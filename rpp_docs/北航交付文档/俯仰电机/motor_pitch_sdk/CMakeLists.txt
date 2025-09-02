cmake_minimum_required(VERSION 3.5)
project(motor_pitch_sdk)
set(version 0.0.1)

include_directories(
    include
)

add_library(${PROJECT_NAME} SHARED
    src/motor_pitch.cpp
    src/can.cpp
)

target_link_libraries(${PROJECT_NAME}
)

add_executable(rpp_keyboard_teleoperate
    examples/keyboard_teleoperate.cpp
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


