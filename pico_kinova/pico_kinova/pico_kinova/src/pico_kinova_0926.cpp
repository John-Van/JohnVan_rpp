#include <cstdio>
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
#include <csignal>

//kinova control
#include <memory>
#include "moveit/move_group_interface/move_group_interface.h"
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

//kinova api
#include <BaseClientRpc.h>
#include <SessionManager.h>

#include <RouterClient.h>
#include <TransportClientTcp.h>

#include <utilities.h>

using json = nlohmann::json;

// Global variables
std::atomic<bool> running{true};
std::atomic<bool> dynamixel_running{true};
std::mutex coutMutex;

// 全局ROS2节点和MoveIt2接口
std::shared_ptr<rclcpp::Node> ros_node;

//kinova设置
namespace k_api = Kinova::Api;

#define PORT 10000

// Maximum allowed waiting time during actions
constexpr auto TIMEOUT_DURATION = std::chrono::seconds(20);

// Create an event listener that will set the promise action event to the exit value
// Will set to either END or ABORT
// Use finish_promise.get_future.get() to wait and get the value
std::function<void(k_api::Base::ActionNotification)> 
    create_action_event_listener_by_promise(std::promise<k_api::Base::ActionEvent>& finish_promise)
{
    return [&finish_promise] (k_api::Base::ActionNotification notification)
    {
        const auto action_event = notification.action_event();
        switch(action_event)
        {
        case k_api::Base::ActionEvent::ACTION_END:
        case k_api::Base::ActionEvent::ACTION_ABORT:
            finish_promise.set_value(action_event);
            break;
        default:
            break;
        }
    };
}

bool example_move_to_home_position(k_api::Base::BaseClient* base)
{
    // Make sure the arm is in Single Level Servoing before executing an Action
    auto servoingMode = k_api::Base::ServoingModeInformation();
    servoingMode.set_servoing_mode(k_api::Base::ServoingMode::SINGLE_LEVEL_SERVOING);
    base->SetServoingMode(servoingMode);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Move arm to ready position
    std::cout << "Moving the arm to a safe position" << std::endl;
    auto action_type = k_api::Base::RequestedActionType();
    action_type.set_action_type(k_api::Base::REACH_JOINT_ANGLES);
    auto action_list = base->ReadAllActions(action_type);
    auto action_handle = k_api::Base::ActionHandle();
    action_handle.set_identifier(0);
    for (auto action : action_list.action_list()) 
    {
        if (action.name() == "Home") 
        {
            action_handle = action.handle();
        }
    }

    if (action_handle.identifier() == 0) 
    {
        std::cout << "Can't reach safe position, exiting" << std::endl;
        return false;
    } 
    else 
    {
        // Connect to notification action topic
        std::promise<k_api::Base::ActionEvent> promise;
        auto future = promise.get_future();
        auto notification_handle = base->OnNotificationActionTopic(
            create_action_event_listener_by_promise(promise),
            k_api::Common::NotificationOptions{}
        );

        base->ExecuteActionFromReference(action_handle);

        // Wait for action to finish
        const auto status = future.wait_for(TIMEOUT_DURATION);
        base->Unsubscribe(notification_handle);

        if(status != std::future_status::ready)
        {
            std::cout << "Timeout on action notification wait" << std::endl;
            return false;
        }

        return true;

    }
}

bool example_twist_command(k_api::Base::BaseClient* base)
{
    auto command = k_api::Base::TwistCommand();
    command.set_reference_frame(k_api::Common::CARTESIAN_REFERENCE_FRAME_TOOL);

    std::cout << "Sending twist command for 2 seconds..." << std::endl;

    auto twist = command.mutable_twist();
    twist->set_linear_x(0.0f);
    twist->set_linear_y(0.03f);
    twist->set_linear_z(0.00f);
    twist->set_angular_x(0.0f);
    twist->set_angular_y(0.0f);
    twist->set_angular_z(0.0f);
    base->SendTwistCommand(command);

    // Let time for twist to be executed
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::cout << "Stopping robot ..." << std::endl;

    // Make movement stop
    base->Stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    return true;
}

bool kinova_twist_command(k_api::Base::BaseClient* base,float linear_x, float linear_y, float linear_z, float angular_x, float angular_y, float angular_z)
{
    auto command = k_api::Base::TwistCommand();
    command.set_reference_frame(k_api::Common::CARTESIAN_REFERENCE_FRAME_TOOL);

    std::cout << "Sending twist command" << std::endl;

    auto twist = command.mutable_twist();
    twist->set_linear_x(linear_x);
    twist->set_linear_y(linear_y);
    twist->set_linear_z(linear_z);
    twist->set_angular_x(angular_x);
    twist->set_angular_y(angular_y);
    twist->set_angular_z(angular_z);
    base->SendTwistCommand(command);

    return true;
}

void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received.\n";
    
    running = false;
    dynamixel_running = false;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::exit(signum);
}

std::array<double, 3> quaternionToEuler(double qx, double qy, double qz, double qw);

std::array<double, 7> LeftControllerPose{0};
std::array<double, 7> RightControllerPose{0};
std::array<double, 7> HeadsetPose{0};
std::array<double, 3> LeftControllerEuler{0};
std::array<double, 3> RightControllerEuler{0};
std::array<double, 3> HeadsetEuler{0};
std::array<double, 6> LastLeftControllerCon{0};
std::array<double, 6> LastRightControllerCon{0};
std::array<double, 6> LastHeadsetCon{0};
std::array<double, 6> LeftControllerVelocity{0};
std::array<double, 6> RightControllerVelocity{0};
std::array<double, 6> HeadsetVelocity{0};
bool LeftRunFlag = false;
bool RightRunFlag = false;
double CurrentTime = 0.0;
double LastTime = 0.0;
double DeltaTime = 0.0;
std::mutex leftPoseMutex;
std::mutex rightPoseMutex;
std::mutex headsetPoseMutex;

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

std::string world_frame = "base_link";
std::string left_ee_frame = "left_end_effector_link";
std::string right_ee_frame = "right_end_effector_link";

geometry_msgs::msg::Twist transformVelocity(
    const geometry_msgs::msg::Twist& hand_twist,
    const geometry_msgs::msg::TransformStamped& transform) {
    
    geometry_msgs::msg::Twist ee_twist;
    
    // 提取旋转矩阵和平移向量
    tf2::Quaternion rotation;
    tf2::fromMsg(transform.transform.rotation, rotation);
    tf2::Matrix3x3 R(rotation);
    
    tf2::Vector3 translation;
    tf2::fromMsg(transform.transform.translation, translation);
    
    // 转换角速度
    tf2::Vector3 omega_hand(
        hand_twist.angular.x,
        hand_twist.angular.y, 
        hand_twist.angular.z);
        
    tf2::Vector3 omega_ee = R * omega_hand;
    
    // 转换线速度 (考虑旋转和杠杆臂效应)
    tf2::Vector3 v_hand(
        hand_twist.linear.x,
        hand_twist.linear.y,
        hand_twist.linear.z);
        
    // v_ee = R * (v_hand + omega_hand × translation)
    tf2::Vector3 v_ee = R * (v_hand + omega_hand.cross(translation));
    
    // 填充结果
    ee_twist.linear.x = v_ee.x();
    ee_twist.linear.y = v_ee.y();
    ee_twist.linear.z = v_ee.z();
    
    ee_twist.angular.x = omega_ee.x();
    ee_twist.angular.y = omega_ee.y();
    ee_twist.angular.z = omega_ee.z();
    
    return ee_twist;
}

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
            // std::cout << "JSON 数据内容:" << std::endl;
            // std::cout << data.dump(4) << std::endl; // 4 是缩进空格数
            if (data.contains("value")) {
                auto value = json::parse(data["value"].get<std::string>());
                // 解析predictTime
                if (value.contains("timeStampNs")) {
                    LastTime = CurrentTime;
                    CurrentTime = value["timeStampNs"].get<double>() * 1e-9; // 转换为秒
                    std::cout << "时间戳 (秒): " << CurrentTime << std::endl;
                    DeltaTime = CurrentTime - LastTime;
                }
                if (value["Controller"].contains("left")) {
                    auto& left = value["Controller"]["left"];
                    {
                        std::lock_guard<std::mutex> lock(leftPoseMutex);
                        //左边手柄的数据
                        LastLeftControllerCon[0] = LeftControllerPose[0];
                        LastLeftControllerCon[1] = LeftControllerPose[1];
                        LastLeftControllerCon[2] = LeftControllerPose[2];
                        LastLeftControllerCon[3] = LeftControllerEuler[0];
                        LastLeftControllerCon[4] = LeftControllerEuler[1];
                        LastLeftControllerCon[5] = LeftControllerEuler[2];
                        // std::cout << "左手手柄" << std::endl;
                        // std::cout << left["pose"].get<std::string>() << std::endl;
                        LeftControllerPose = stringToPoseArray(left["pose"].get<std::string>());
                        LeftControllerEuler = quaternionToEuler(LeftControllerPose[3], LeftControllerPose[4], LeftControllerPose[5], LeftControllerPose[6]);
                        LeftTrigger = left["trigger"].get<double>();
                        LeftGrip = left["grip"].get<double>();
                        std::cout << "左手手柄触发器: " << LeftTrigger.load() << ", 握持: " << LeftGrip.load() << std::endl;
                        //计算速度
                        if (DeltaTime > 0) {
                            for (int i = 0; i < 3; ++i) {
                                LeftControllerVelocity[i] = (LeftControllerPose[i] - LastLeftControllerCon[i]) / DeltaTime;
                                LeftControllerVelocity[i + 3] = (LeftControllerEuler[i] - LastLeftControllerCon[i + 3]) / DeltaTime;
                            }
                        }
                        //输出速度
                        std::cout << "左手手柄速度 (m/s, rad/s): ";
                        for (const auto& v : LeftControllerVelocity) {
                            std::cout << std::fixed << std::setprecision(4) << v << " ";
                        }
                        std::cout << std::endl;

                        //新数据接收标志位
                        LeftRunFlag = true;
                    }
                }
                if (value["Controller"].contains("right")) {
                    auto& right = value["Controller"]["right"];
                    {
                        std::lock_guard<std::mutex> lock(rightPoseMutex);
                        //右边手柄的数据
                        LastRightControllerCon[0] = RightControllerPose[0];
                        LastRightControllerCon[1] = RightControllerPose[1];
                        LastRightControllerCon[2] = RightControllerPose[2];
                        LastRightControllerCon[3] = RightControllerEuler[0];
                        LastRightControllerCon[4] = RightControllerEuler[1];
                        LastRightControllerCon[5] = RightControllerEuler[2];
                        // std::cout << "右手手柄" << std::endl;
                        // std::cout << right["pose"].get<std::string>() << std::endl;
                        RightControllerPose = stringToPoseArray(right["pose"].get<std::string>());
                        RightControllerEuler = quaternionToEuler(RightControllerPose[3], RightControllerPose[4], RightControllerPose[5], RightControllerPose[6]);
                        RightTrigger = right["trigger"].get<double>();
                        RightGrip = right["grip"].get<double>();
                        std::cout << "右手手柄触发器: " << RightTrigger.load() << ", 握持: " << RightGrip.load() << std::endl;
                        //计算速度
                        if (DeltaTime > 0) {
                            for (int i = 0; i < 3; ++i) {
                                RightControllerVelocity[i] = (RightControllerPose[i] - LastRightControllerCon[i]) / DeltaTime;
                                RightControllerVelocity[i + 3] = (RightControllerEuler[i] - LastRightControllerCon[i + 3]) / DeltaTime;
                            }
                        }
                        //输出速度
                        // std::cout << "右手手柄速度 (m/s, rad/s): ";
                        // for (const auto& v : RightControllerVelocity) {
                        //     std::cout << std::fixed << std::setprecision(4) << v << " ";
                        // }
                        // std::cout << std::endl;

                        //新数据接收标志位
                        RightRunFlag = true;
                    }
                }
                if (value.contains("Head")) {
                    auto& headset = value["Head"];
                    {
                        std::lock_guard<std::mutex> lock(headsetPoseMutex);
                        //头戴设备的数据
                        LastHeadsetCon[0] = HeadsetPose[0];
                        LastHeadsetCon[1] = HeadsetPose[1];
                        LastHeadsetCon[2] = HeadsetPose[2];
                        LastHeadsetCon[3] = HeadsetEuler[0];
                        LastHeadsetCon[4] = HeadsetEuler[1];
                        LastHeadsetCon[5] = HeadsetEuler[2];
                        // std::cout << "头戴设备" << std::endl;
                        // std::cout << headset["pose"].get<std::string>() << std::endl;
                        HeadsetPose = stringToPoseArray(headset["pose"].get<std::string>());
                        HeadsetEuler = quaternionToEuler(HeadsetPose[3], HeadsetPose[4], HeadsetPose[5], HeadsetPose[6]);
                        //计算速度
                        if (DeltaTime > 0) {
                            for (int i = 0; i < 3; ++i) {
                                HeadsetVelocity[i] = (HeadsetPose[i] - LastHeadsetCon[i]) / DeltaTime;
                                HeadsetVelocity[i + 3] = (HeadsetEuler[i] - LastHeadsetCon[i + 3]) / DeltaTime;
                            }
                        }
                        //输出速度
                        // std::cout << "头戴设备速度 (m/s, rad/s): ";
                        for (const auto& v : HeadsetVelocity) {
                            std::cout << std::fixed << std::setprecision(4) << v << " ";
                        }
                        std::cout << std::endl;
                    }
                }
            }
        } catch (const json::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
        }
        break;
    }
}

int main(int argc, char *argv[])
{
    // Register signal handler
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    // ROS2初始化
    rclcpp::init(argc, argv);
    ros_node = rclcpp::Node::make_shared("vr_kinova_control");

    // 成员变量
    tf2_ros::Buffer tf_buffer_ = tf2_ros::Buffer(ros_node->get_clock());
    tf2_ros::TransformListener tf_listener_ = tf2_ros::TransformListener(tf_buffer_);

    //开启VR数据接收
    PXREAInit(NULL, OnPXREAClientCallback, PXREAFullMask);

// -------------------------------------------------------------
    auto parsed_args_right = ParseExampleArguments_right(argc, argv);
    auto parsed_args_left = ParseExampleArguments_left(argc, argv);

    //right arm Api objects
    auto error_callback_right = [](k_api::KError err){ cout << "_________ callback error _________" << err.toString(); };
    auto transport_right = new k_api::TransportClientTcp();
    auto router_right = new k_api::RouterClient(transport_right, error_callback_right);
    transport_right->connect(parsed_args_right.ip_address, PORT);
    //left arm Api objects
    auto error_callback_left = [](k_api::KError err){ cout << "_________ callback error _________" << err.toString(); };
    auto transport_left = new k_api::TransportClientTcp();
    auto router_left = new k_api::RouterClient(transport_left, error_callback_left);
    transport_left->connect(parsed_args_left.ip_address, PORT); 

    // Set right_session data connection information
    auto create_session_info_right = k_api::Session::CreateSessionInfo();
    create_session_info_right.set_username(parsed_args_right.username);
    create_session_info_right.set_password(parsed_args_right.password);
    create_session_info_right.set_session_inactivity_timeout(60000);   // (milliseconds)
    create_session_info_right.set_connection_inactivity_timeout(2000); // (milliseconds)

    // Set left_session data connection information
    auto create_session_info_left = k_api::Session::CreateSessionInfo();
    create_session_info_left.set_username(parsed_args_left.username);
    create_session_info_left.set_password(parsed_args_left.password);
    create_session_info_left.set_session_inactivity_timeout(60000);   // (milliseconds)
    create_session_info_left.set_connection_inactivity_timeout(2000); // (milliseconds)

    //right arm Session manager service wrapper
    std::cout << "Creating right arm session for communication" << std::endl;
    auto session_manager_right = new k_api::SessionManager(router_right);
    session_manager_right->CreateSession(create_session_info_right);
    std::cout << "Right arm session created" << std::endl;
    //left arm Session manager service wrapper
    std::cout << "Creating left arm session for communication" << std::endl;
    auto session_manager_left = new k_api::SessionManager(router_left);
    session_manager_left->CreateSession(create_session_info_left);
    std::cout << "Left arm session created" << std::endl;

    // Create right arm services
    auto base_right = new k_api::Base::BaseClient(router_right);
    // Create left arm services
    auto base_left = new k_api::Base::BaseClient(router_left);
    
    // Example core
    // 运行到初始位置，并进行伺服模式的切换
    example_move_to_home_position(base_right);
    example_move_to_home_position(base_left);

    try {
        // 主循环或主要逻辑
        while (running) { // 根据您的实际循环条件调整
            // 获取时间戳
            auto CurrentTime = ros_node->now();

            // 获取从世界坐标系到末端执行器坐标系的变换
            geometry_msgs::msg::TransformStamped left_transform;
            left_transform = tf_buffer_.lookupTransform(
                left_ee_frame, world_frame, CurrentTime, 
                rclcpp::Duration::from_seconds(0.1));
            
            // 将全局变量数据转换为geometry_msgs::Twist
            geometry_msgs::msg::Twist left_hand_twist;
            left_hand_twist.linear.x = LeftControllerVelocity[0] ;
            left_hand_twist.linear.y = LeftControllerVelocity[1] ;
            left_hand_twist.linear.z = LeftControllerVelocity[2] ;
            left_hand_twist.angular.x = LeftControllerVelocity[3] ;
            left_hand_twist.angular.y = LeftControllerVelocity[4] ;
            left_hand_twist.angular.z = LeftControllerVelocity[5] ;
            // 转换速度到末端执行器坐标系
            auto left_ee_twist = transformVelocity(left_hand_twist, left_transform);
            // 输出转换后的速度
            std::cout << "Left EE Twist: "
                      << "Linear(" << left_ee_twist.linear.x << ", "
                                   << left_ee_twist.linear.y << ", "
                                   << left_ee_twist.linear.z << ") "
                      << "Angular(" << left_ee_twist.angular.x << ", "
                                    << left_ee_twist.angular.y << ", "
                                    << left_ee_twist.angular.z << ")"
                      << std::endl;

            // 检查LeftGrip
            if (LeftGrip != 0.0 && LeftRunFlag) {
                //发送速度指令
                kinova_twist_command(base_left, 
                                   left_ee_twist.linear.x, 
                                   left_ee_twist.linear.y, 
                                   left_ee_twist.linear.z, 
                                   left_ee_twist.angular.x, 
                                   left_ee_twist.angular.y, 
                                   left_ee_twist.angular.z);
                LeftRunFlag = false;
            }
            else {
                base_left->Stop();
            }
            
            // RightGrip
            if (RightGrip != 0.0 && RightRunFlag) {
                //发送速度指令
                kinova_twist_command(base_right, 
                                   RightControllerVelocity[2], 
                                   RightControllerVelocity[0], 
                                   RightControllerVelocity[1], 
                                   RightControllerVelocity[5],
                                   RightControllerVelocity[3],
                                   RightControllerVelocity[4]);
            }
            else {
                base_right->Stop();
            }

            //添加50ms的延时
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            
            // 这里可以添加适当的延迟或退出条件
            // 根据实际情况退出循环
        }
    }
    catch (const std::exception& e) {
        std::cerr << "程序发生异常: " << e.what() << std::endl;
            // Close API session
        session_manager_right->CloseSession();
        session_manager_left->CloseSession();

        // Deactivate the router and cleanly disconnect from the transport object
        router_right->SetActivationStatus(false);
        transport_right->disconnect();
        router_left->SetActivationStatus(false);
        transport_left->disconnect();

        // Destroy the API
        delete base_right;
        delete session_manager_right;
        delete router_right;
        delete transport_right;
        delete base_left;
        delete session_manager_left;
        delete router_left;
        delete transport_left;

        return -1;
    }
    catch (...) {
        std::cerr << "程序发生未知异常" << std::endl;
            // Close API session
        session_manager_right->CloseSession();
        session_manager_left->CloseSession();

        // Deactivate the router and cleanly disconnect from the transport object
        router_right->SetActivationStatus(false);
        transport_right->disconnect();
        router_left->SetActivationStatus(false);
        transport_left->disconnect();

        // Destroy the API
        delete base_right;
        delete session_manager_right;
        delete router_right;
        delete transport_right;
        delete base_left;
        delete session_manager_left;
        delete router_left;
        delete transport_left;

        return -1;
    }

// -------------------------------------------------------------

    PXREADeinit();
    running = false;
    dynamixel_running = false;

    rclcpp::shutdown();

    return 0;
}

