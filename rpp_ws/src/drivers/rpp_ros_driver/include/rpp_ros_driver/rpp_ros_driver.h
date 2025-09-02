#ifndef __H_RPP_ROS_DRIVER_H__
#define __H_RPP_ROS_DRIVER_H__

#include <string>
#include <rclcpp/rclcpp.hpp>

#include "rpp_driver/rpp_driver.h"

#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"

#include "rpp_msgs/msg/device_state.hpp"
#include "rpp_msgs/msg/light_state.hpp"
#include "rpp_msgs/msg/battery_state.hpp"
#include "rpp_msgs/msg/error_state.hpp"
#include "rpp_msgs/msg/motor_state.hpp"

#include "rpp_msgs/msg/set_device.hpp"
#include "rpp_msgs/msg/set_error_mask.hpp"
#include "rpp_msgs/msg/set_light.hpp"

// clang-format off
const double odom_pose_covariance[36] = 
    {1e-3, 0, 0, 0, 0, 0,
    0, 1e-3, 0, 0, 0, 0,
    0, 0, 1e6, 0, 0, 0,
    0, 0, 0, 1e6, 0, 0,
    0, 0, 0, 0, 1e6, 0,
    0, 0, 0, 0, 0, 1e3};

const double odom_pose_covariance2[36] = 
    {1e-9, 0, 0, 0, 0, 0,
    0, 1e-3, 1e-9, 0, 0, 0,
    0, 0, 1e6, 0, 0, 0,
    0, 0, 0, 1e6, 0, 0,
    0, 0, 0, 0, 1e6, 0,
    0, 0, 0, 0, 0, 1e-9};

const double odom_twist_covariance[36] = 
    {1e-3, 0, 0, 0, 0, 0,
    0, 1e-3, 0, 0, 0, 0,
    0, 0, 1e6, 0, 0, 0,
    0, 0, 0, 1e6, 0, 0,
    0, 0, 0, 0, 1e6, 0,
    0, 0, 0, 0, 0, 1e3};

const double odom_twist_covariance2[36] = 
    {1e-9, 0, 0, 0, 0, 0,
    0, 1e-3, 1e-9, 0, 0, 0,
    0, 0, 1e6, 0, 0, 0,
    0, 0, 0, 1e6, 0, 0,
    0, 0, 0, 0, 1e6, 0,
    0, 0, 0, 0, 0, 1e-9};
// clang-format on

namespace rpp
{

class RPPRosDriver : public rclcpp::Node
{
  public:
    RPPRosDriver();
    ~RPPRosDriver();

    void setDeviceCallback(const rpp_msgs::msg::SetDevice::SharedPtr cmd);

    void setLightCallback(const rpp_msgs::msg::SetLight::SharedPtr cmd);

    void setErrorMaskCallback(const rpp_msgs::msg::SetErrorMask::SharedPtr cmd);

    void twistCallback(const geometry_msgs::msg::Twist::SharedPtr cmd);

    void publishData();

    RPPDriver *rpp_driver_;

    // param
    std::string port_name_;
    int baud_rate_;
    int control_rate_;

    std::string motion_model_; // 2wd 4wd ackermann mecanum 4w4s
    double wheel_radius_;
    double wheel_bias_;
    double wheel_base_;
    bool use_diff_twist_;

    std::string odom_frame_;
    std::string base_frame_;
    bool publish_odom_;
    bool publish_odom_tf_;

    // data, only in timer, one thread, don't need mutex
    rpp_msgs::msg::DeviceState device_state_;
    rpp_msgs::msg::BatteryState battery_state_;
    rpp_msgs::msg::LightState light_state_;
    rpp_msgs::msg::ErrorState error_state_;
    rpp_msgs::msg::MotorState motor_state_;
    rpp_msgs::msg::MotorState trun_motor_state_;
    geometry_msgs::msg::Twist twist_;
    nav_msgs::msg::Odometry odom_;

    // cmd sub
    rclcpp::Subscription<rpp_msgs::msg::SetDevice>::SharedPtr set_mode_sub_;
    rclcpp::Subscription<rpp_msgs::msg::SetLight>::SharedPtr set_light_sub_;
    rclcpp::Subscription<rpp_msgs::msg::SetErrorMask>::SharedPtr set_error_mask_sub_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr twist_sub_;
    rclcpp::Time last_twist_time_;

    // data pub
    rclcpp::TimerBase::SharedPtr publish_data_timer_;
    rclcpp::Time cur_time_;
    rclcpp::Publisher<rpp_msgs::msg::DeviceState>::SharedPtr device_pub_;
    rclcpp::Publisher<rpp_msgs::msg::BatteryState>::SharedPtr battery_pub_;
    rclcpp::Publisher<rpp_msgs::msg::LightState>::SharedPtr light_pub_;
    rclcpp::Publisher<rpp_msgs::msg::ErrorState>::SharedPtr error_pub_;
    rclcpp::Publisher<rpp_msgs::msg::MotorState>::SharedPtr motor_pub_;
    rclcpp::Publisher<rpp_msgs::msg::MotorState>::SharedPtr trun_motor_pub_;

    // odom pub
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr twist_pub_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
    std::unique_ptr<tf2_ros::TransformBroadcaster> odom_broadcaster_;
};

} // namespace rpp

#endif // __H_RPP_ROS_DRIVER_H__