#ifndef CONFIG_H
#define CONFIG_H

// Dynamixel motor parameters
#define MOTOR_ID 3
#define BAUDRATE 4500000
#define DEVICE_NAME "/dev/ttyUSB0"  // Change this to your port name

// Protocol version
#define PROTOCOL_VERSION 2.0

// Control table address
#define ADDR_LED_RED 65
#define ADDR_TORQUE_ENABLE 64
#define ADDR_GOAL_POSITION 116
#define ADDR_PRESENT_POSITION 132

// Dynamixel motor IDs
#define YAW_MOTOR_ID 3
#define PITCH_MOTOR_ID 1

// Dynamixel position constants
#define YAW_CENTER 1521
#define PITCH_CENTER 2753
#define DYNAMIXEL_DEGREE_PER_UNIT 0.0879

// UR5 robot IP addresses
#define LEFT_ROBOT_IP "192.168.50.55"
#define RIGHT_ROBOT_IP "192.168.50.195"

// UR5 control parameters
#define SERVO_TIME 0.017        // 17ms (60Hz)
#define LOOKAHEAD_TIME 0.1      // 100ms look ahead
#define SERVO_GAIN 300          // Servo gain
#define MAX_VELOCITY 0.5        // 0.5 m/s
#define MAX_ACCELERATION 1.0    // 1.0 m/s^2

// Gripper parameters
#define GRIPPER_FORCE 0.5f
#define GRIPPER_SPEED 1.0f

#endif // CONFIG_H