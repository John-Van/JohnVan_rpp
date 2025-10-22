#define _USE_MATH_DEFINES
#include <cmath>
#include <stddef.h>
#include <iostream>
#include <PXREARobotSDK.h>
#include <chrono>
#include <thread>
#include <string>
#include <array>
#include <nlohmann/json.hpp>
#include <sstream>
#include <mutex>
#include <atomic>
#include <iomanip>
#include <Eigen/Geometry>
#include <ur_rtde/rtde_control_interface.h>
#include <ur_rtde/rtde_receive_interface.h>
#include <ur_rtde/robotiq_gripper.h>
#include "dynamixel_sdk.h"  // Dynamixel SDK header
#include <csignal>

//kinova control
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "moveit/move_group_interface/move_group_interface.h"
// #include <geometry_msgs/msg/pose.hpp>

using json = nlohmann::json;

// Dynamixel SDK namespace
using namespace dynamixel;

// Dynamixel motor parameters
#define MOTOR_ID 3
#define BAUDRATE 4500000
#define DEVICE_NAME "/dev/ttyUSB0"//"COM3"  // Change this to your port name

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

// Global variables
std::atomic<bool> running{true};
std::atomic<bool> dynamixel_running{true};
std::mutex dynamixel_mutex;
std::mutex coutMutex;
PortHandler* portHandler = nullptr;
PacketHandler* packetHandler = nullptr;

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";
    
    running = false;
    dynamixel_running = false;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::exit(signum);
}

int32_t mapYawToDynamixelPosition(double yaw) {
    // Map yaw from [-90, 90] to Dynamixel position
    int32_t position = YAW_CENTER + static_cast<int32_t>(yaw / DYNAMIXEL_DEGREE_PER_UNIT);
    std::cout << "Yaw Dynamixel position: " << position << std::endl;
    return position;
}

// Function to map pitch angle to Dynamixel position
int32_t mapPitchToDynamixelPosition(double pitch) {
    // Map pitch from [-50, 50] to Dynamixel position
    
    if (pitch > 50) pitch = 50;
    if (pitch < -50) pitch = -50;
    
    int32_t position = PITCH_CENTER - static_cast<int32_t>(pitch / DYNAMIXEL_DEGREE_PER_UNIT);
    std::cout << "Pitch Dynamixel position: " << position << std::endl;
    return position;
}

// std::vector<double> degreesToRadians(const std::vector<double>& degrees);
std::array<double, 3> quaternionToEuler(double qx, double qy, double qz, double qw);
std::vector<double> convertControllerToUR5PoseLeft(const std::array<double, 7>& controllerPose);
std::vector<double> convertControllerToUR5PoseRight(const std::array<double, 7>& controllerPose);
std::vector<double> calculateRelativePoseChangeLeft(const std::array<double, 7>& currentPose, 
                                              const std::array<double, 7>& previousPose);
std::vector<double> calculateRelativePoseChangeRight(const std::array<double, 7>& currentPose, 
                                              const std::array<double, 7>& previousPose);
bool isValidControllerPoseLeft(const std::array<double, 7>& pose);
bool isValidControllerPoseRight(const std::array<double, 7>& pose);
void leftUR5Control();
void rightUR5Control();
void leftConnectionMonitor();
void dynamixelControl();

std::array<double, 7> LeftControllerPose{0};
std::array<double, 7> RightControllerPose{0};
std::array<double, 7> HeadsetPose{0};
std::mutex leftPoseMutex;
std::mutex rightPoseMutex;
std::mutex headsetPoseMutex;

// 设置机械臂的ip地址
const std::string LEFT_ROBOT_IP = "192.168.50.55"; 
const std::string RIGHT_ROBOT_IP = "192.168.50.195";

constexpr double DEG2RAD = M_PI / 180.0;
const double SERVO_TIME = 0.017;        // 17ms (60Hz)
const double LOOKAHEAD_TIME = 0.1;      // 100ms look ahead
const double SERVO_GAIN = 300;          // Servo gain
const double MAX_VELOCITY = 0.5;        // 0.5 m/s
const double MAX_ACCELERATION = 1.0;    // 1.0 m/s^2

std::atomic<double> LeftTrigger{0.0};
std::atomic<double> RightTrigger{0.0};
std::atomic<double> LeftGrip{0.0};
std::atomic<double> RightGrip{0.0};

const float GRIPPER_FORCE = 0.5f;
const float GRIPPER_SPEED = 1.0f;

std::atomic<bool> leftConnectionOK{true};
std::mutex debugMutex;

const std::vector<double> LEFT_INITIAL_JOINT_DEG = {165.26, -47.50, 118.93, -38.96, 87.51, 149.56};
const std::vector<double> RIGHT_INITIAL_JOINT_DEG = {193.53, -164.17, -114.02, 58.01, 101.87, -138.40};

// const std::vector<double> LEFT_INITIAL_JOINT = degreesToRadians(LEFT_INITIAL_JOINT_DEG);
// const std::vector<double> RIGHT_INITIAL_JOINT = degreesToRadians(RIGHT_INITIAL_JOINT_DEG);

// 全局ROS2节点和MoveIt2接口
// std::shared_ptr<rclcpp::Node> ros_node;
// std::unique_ptr<moveit::planning_interface::MoveGroupInterface> left_move_group;
// std::unique_ptr<moveit::planning_interface::MoveGroupInterface> right_move_group;

std::array<double, 7> stringToPoseArray(const std::string& poseStr) {
    std::array<double, 7> result{0};
    std::stringstream ss(poseStr);
    std::string value;
    int i = 0;
    while (std::getline(ss, value, ',') && i < 7) {
        result[i++] = std::stod(value);
    }
    return result;
}

bool isValidControllerPoseLeft(const std::array<double, 7>& pose) {
    return pose[0] != 0.0;  // Check if x position is not 0
}

bool isValidControllerPoseRight(const std::array<double, 7>& pose) {
    return pose[0] != 0.0;  // Check if x position is not 0
}

std::array<double, 3> quaternionToEuler(double qx, double qy, double qz, double qw) {
    std::array<double, 3> euler;
    
    
    // Transformation: x to z, y to x, z to y
    double transformed_qx = qz; 
    double transformed_qy = qx;
    double transformed_qz = qy; 

    // Roll (x-axis rotation)
    double sinr_cosp = 2 * (qw * transformed_qx + transformed_qy * transformed_qz);
    double cosr_cosp = 1 - 2 * (transformed_qx * transformed_qx + transformed_qy * transformed_qy);
    euler[0] = std::atan2(sinr_cosp, cosr_cosp);
    
    // Pitch (y-axis rotation)
    double sinp = 2 * (qw * transformed_qy - transformed_qz * transformed_qx);
    if (std::abs(sinp) >= 1)
        euler[1] = std::copysign(M_PI / 2, sinp);
    else
        euler[1] = std::asin(sinp);
    
    // Yaw (z-axis rotation)
    double siny_cosp = 2 * (qw * transformed_qz + transformed_qx * transformed_qy);
    double cosy_cosp = 1 - 2 * (transformed_qy * transformed_qy + transformed_qz * transformed_qz);
    euler[2] = std::atan2(siny_cosp, cosy_cosp);
    
    return euler;
}

std::array<double, 3> eulerToRobotRotVectorLeft(const std::array<double, 3>& euler) {

    double rx = euler[1];
    double ry = euler[2];
    double rz = euler[0];
    
    const double cos45 = 0.70710678118;
    const double sin45 = 0.70710678118;
    
    double ry_rotated = ry * cos45 + rz * sin45;
    double rz_rotated = -ry * sin45 + rz * cos45;
    
    rx = rx;
    ry = ry;
    rz = rz;
    
    double cx = std::cos(rx * 0.5);
    double sx = std::sin(rx * 0.5);
    double cy = std::cos(ry * 0.5);
    double sy = std::sin(ry * 0.5);
    double cz = std::cos(rz * 0.5);
    double sz = std::sin(rz * 0.5);
    
    double qw = cx*cy*cz - sx*sy*sz;
    double qx = sx*cy*cz + cx*sy*sz;
    double qy = cx*sy*cz - sx*cy*sz;
    double qz = cx*cy*sz + sx*sy*cz;

    std::array<double, 3> rotVec{0, 0, 0};
    double angle = 2.0 * std::acos(qw);
    
    if (std::abs(angle) > 1e-6) {
        double s = std::sqrt(1 - qw * qw);
        if (std::abs(s) > 1e-6) {
            rotVec[0] = (qx / s) * angle;
            rotVec[1] = (qy / s) * angle;
            rotVec[2] = (qz / s) * angle;
        }
    }
    
    return rotVec;
}

std::array<double, 3> eulerToRobotRotVectorRight(const Eigen::Vector3d& eigenEuler) {
    std::array<double, 3> euler{eigenEuler.x(), eigenEuler.y(), eigenEuler.z()};
    
    double alpha = euler[0];
    double beta = euler[1];
    double gamma = euler[2];
    double ca = std::cos(alpha);
    double cb = std::cos(beta);
    double cg = std::cos(gamma);
    double sa = std::sin(alpha);
    double sb = std::sin(beta);
    double sg = std::sin(gamma);
    double r11 = ca*cb;
    double r12 = ca*sb*sg - sa*cg;
    double r13 = ca*sb*cg + sa*sg;
    double r21 = sa*cb;
    double r22 = sa*sb*sg + ca*cg;
    double r23 = sa*sb*cg - ca*sg;
    double r31 = -sb;
    double r32 = cb*sg;
    double r33 = cb*cg;
    double theta = std::acos((r11 + r22 + r33 - 1.0) * 0.5);
    double sth = std::sin(theta);
    double kx = (r32-r23)/(2*sth);
    double ky = (r13-r31)/(2*sth);
    double kz = (r21-r12)/(2*sth);
    std::array<double, 3> rotVec{0, 0, 0};
    rotVec[0] = theta*kx;
    rotVec[1] = theta*ky;
    rotVec[2] = theta*kz;
    
    return rotVec;
}

std::vector<double> convertControllerToUR5PoseRight(const std::array<double, 7>& controllerPose) {
    std::vector<double> ur5Pose(6);
    
    double x = controllerPose[2];
    double y = controllerPose[0];
    double z = controllerPose[1];
    
    const double cos45 = 0.70710678118;
    const double sin45 = 0.70710678118;
    
    double y_rotated = y * cos45 - z * sin45;
    double z_rotated = y * sin45 + z * cos45;
    
    ur5Pose[0] = x;
    ur5Pose[1] = y_rotated;
    ur5Pose[2] = z_rotated;
    
    Eigen::Quaterniond q_controller(controllerPose[6], controllerPose[3], controllerPose[4], controllerPose[5]);
    Eigen::Quaterniond rotX_controller(Eigen::AngleAxisd(-M_PI/2, Eigen::Vector3d::UnitX()));
    Eigen::Quaterniond q_controller_rotated = rotX_controller * q_controller;

    Eigen::Quaterniond q_robot(q_controller_rotated.w(), q_controller_rotated.y(), q_controller_rotated.x(), -q_controller_rotated.z());
    Eigen::Quaterniond rotX_robot(Eigen::AngleAxisd(M_PI/4, Eigen::Vector3d::UnitX()));
    Eigen::Quaterniond q_robot_rotated = rotX_robot * q_robot;

    Eigen::Matrix3d rotMatrix = q_robot_rotated.toRotationMatrix();
    Eigen::Vector3d euler = rotMatrix.eulerAngles(2, 1, 0);
    auto rotVec = eulerToRobotRotVectorRight(euler);
    
    ur5Pose[3] = rotVec[0];
    ur5Pose[4] = rotVec[1];
    ur5Pose[5] = rotVec[2];
    
    return ur5Pose;
}

// 实时接收VR数据的回调函数
void OnPXREAClientCallback(void* context, PXREAClientCallbackType type, int status, void* userData)
{
    switch (type)
    {
    case PXREAServerConnect:
        std::cout << "server connect" << std::endl;
        break;
    case PXREAServerDisconnect:
        std::cout << "server disconnect" << std::endl;
        break;
    case PXREADeviceFind:
        std::cout << "device find" << (const char*)userData << std::endl;
        break;
    case PXREADeviceMissing:
        std::cout << "device missing" << (const char*)userData << std::endl;
        break;
    case PXREADeviceConnect:
        std::cout << "device connect" << (const char*)userData << status << std::endl;
        break;
    //下面这一情况是解析JSON数据，提取控制器的信息，并用互斥锁保护共享数据
    case PXREADeviceStateJson:
        auto& dsj = *((PXREADevStateJson*)userData);
        {
            std::lock_guard<std::mutex> lock(coutMutex);
        }
        try {
            json data = json::parse(dsj.stateJson);
            std::cout << "JSON 数据内容:" << std::endl;
            std::cout << data.dump(4) << std::endl; // 4 是缩进空格数
            if (data.contains("value")) {
                auto value = json::parse(data["value"].get<std::string>());
                if (value["Controller"].contains("left")) {
                    auto& left = value["Controller"]["left"];
                    {
                        std::lock_guard<std::mutex> lock(leftPoseMutex);
                        //左边手柄的数据
                        std::cout << "左手手柄" << std::endl;
                        std::cout << left["pose"].get<std::string>() << std::endl;
                        LeftControllerPose = stringToPoseArray(left["pose"].get<std::string>());
                        LeftTrigger = left["trigger"].get<double>();
                        LeftGrip = left["grip"].get<double>();
                    }
                }
                if (value["Controller"].contains("right")) {
                    auto& right = value["Controller"]["right"];
                    {
                        std::lock_guard<std::mutex> lock(rightPoseMutex);
                        //右边手柄的数据
                        std::cout << "右手手柄" << std::endl;
                        std::cout << right["pose"].get<std::string>() << std::endl;
                        RightControllerPose = stringToPoseArray(right["pose"].get<std::string>());
                        RightTrigger = right["trigger"].get<double>();
                        RightGrip = right["grip"].get<double>();
                    }
                }
                if (value.contains("Head")) {
                    auto& headset = value["Head"];
                    {
                        std::lock_guard<std::mutex> lock(headsetPoseMutex);
                        //头戴设备的数据
                        std::cout << "头戴设备" << std::endl;
                        std::cout << headset["pose"].get<std::string>() << std::endl;
                        HeadsetPose = stringToPoseArray(headset["pose"].get<std::string>());
                    }
                }
            }
        } catch (const json::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
        }
        break;
    }
}

std::vector<double> convertControllerToUR5PoseLeft(const std::array<double, 7>& controllerPose) {
    std::vector<double> ur5Pose(6);
    
    return ur5Pose;
}
// 计算相对位姿变化，位置用差值，姿态用绝对值
std::vector<double> calculateRelativePoseChangeLeft(const std::array<double, 7>& currentPose, 
                                              const std::array<double, 7>& previousPose) {
    std::vector<double> relativePose(6);
    auto current = convertControllerToUR5PoseLeft(currentPose);
    auto previous = convertControllerToUR5PoseLeft(previousPose);
    
    // Calculate position differences (first 3 elements)
    for (int i = 0; i < 3; ++i) {
        relativePose[i] = current[i] - previous[i];
    }
    
    // Use absolute values for orientation (last 3 elements)
    for (int i = 3; i < 6; ++i) {
        relativePose[i] = current[i];
    }
    
    return relativePose;
}
// 计算相对位姿变化，位置用差值，姿态用绝对值
std::vector<double> calculateRelativePoseChangeRight(const std::array<double, 7>& currentPose, 
                                               const std::array<double, 7>& previousPose) {
    std::vector<double> relativePose(6);
    auto current = convertControllerToUR5PoseRight(currentPose);
    auto previous = convertControllerToUR5PoseRight(previousPose);
    
    // Calculate position differences (first 3 elements)
    for (int i = 0; i < 3; ++i) {
        relativePose[i] = current[i] - previous[i];
    }
    
    // Use absolute values for orientation (last 3 elements)
    for (int i = 3; i < 6; ++i) {
        relativePose[i] = current[i];
    }
    
    return relativePose;
}

// VR手柄数据转ROS2 Pose
// geometry_msgs::msg::Pose vrPoseToRosPose(const std::array<double, 7>& vr_pose)
// {
//     geometry_msgs::msg::Pose pose;
//     pose.position.x = vr_pose[0];
//     pose.position.y = vr_pose[1];
//     pose.position.z = vr_pose[2];
//     pose.orientation.x = vr_pose[3];
//     pose.orientation.y = vr_pose[4];
//     pose.orientation.z = vr_pose[5];
//     pose.orientation.w = vr_pose[6];
//     return pose;
// }

// void leftKinovaControl()
// {
//     while (running)
//     {
//         std::array<double, 7> left_pose;
//         {
//             std::lock_guard<std::mutex> lock(leftPoseMutex);
//             left_pose = LeftControllerPose;
//         }
//         geometry_msgs::msg::Pose target_pose = vrPoseToRosPose(left_pose);

//         // 可选：设置速度和加速度缩放因子
//         left_move_group->setMaxVelocityScalingFactor(0.2);
//         left_move_group->setMaxAccelerationScalingFactor(0.2);

//         left_move_group->setPoseTarget(target_pose);
//         left_move_group->move();
//         std::this_thread::sleep_for(std::chrono::milliseconds(50));
//     }
// }

// void rightKinovaControl()
// {
//     while (running)
//     {
//         std::array<double, 7> right_pose;
//         {
//             std::lock_guard<std::mutex> lock(rightPoseMutex);
//             right_pose = RightControllerPose;
//         }
//         geometry_msgs::msg::Pose target_pose = vrPoseToRosPose(right_pose);

//         // 可选：设置速度和加速度缩放因子
//         right_move_group->setMaxVelocityScalingFactor(0.2);
//         right_move_group->setMaxAccelerationScalingFactor(0.2);

//         right_move_group->setPoseTarget(target_pose);
//         right_move_group->move();
//         std::this_thread::sleep_for(std::chrono::milliseconds(50));
//     }
// }

int main(int argc, char *argv[])
{
    // Register signal handler
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // ROS2初始化
    // rclcpp::init(argc, argv);
    // ros_node = rclcpp::Node::make_shared("vr_kinova_control");
    // left_move_group = std::make_unique<moveit::planning_interface::MoveGroupInterface>(ros_node, "left_arm");
    // right_move_group = std::make_unique<moveit::planning_interface::MoveGroupInterface>(ros_node, "right_arm");

    //开启VR数据接收
    PXREAInit(NULL, OnPXREAClientCallback, PXREAFullMask);

    // // 启动机械臂控制线程
    // std::thread leftKinovaThread(leftKinovaControl);
    // std::thread rightKinovaThread(rightKinovaControl);

    while(running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // ...头显姿态输出...
    }
    PXREADeinit();
    running = false;
    dynamixel_running = false;

    // leftKinovaThread.join();
    // rightKinovaThread.join();

    // rclcpp::shutdown();
    return 0;
}
