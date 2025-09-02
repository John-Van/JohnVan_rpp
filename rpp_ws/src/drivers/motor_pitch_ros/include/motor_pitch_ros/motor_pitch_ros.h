#ifndef __TI5CRA_SERVO_ROS_H__
#define __TI5CRA_SERVO_ROS_H__

#include <string>
#include <rclcpp/rclcpp.hpp>

#include "motor_pitch/motor_pitch.h"
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_srvs/srv/set_bool.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include <functional>

namespace rpp
{

class TI5CRA_Servo_ROS : public rclcpp::Node
{
public:
  TI5CRA_Servo_ROS();
  ~TI5CRA_Servo_ROS();

private:
  Ti5CRA_Servo* rpp_driver_;

  // param
  std::string can_port_;
  char can_id_;
  float velocity_set_;
  std::string joint_name_;

  bool show_inplace_ = false;

  // cmd sub
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr set_position_sub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr set_velocity_sub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr set_move_sub_;

  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr set_stop_srv_;
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr set_run_srv_;

  // data pub
  rclcpp::TimerBase::SharedPtr publish_data_timer_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr position_pub_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_;

  void setPositionCallback(const std_msgs::msg::Float32::SharedPtr cmd);
  void setVelocityCallback(const std_msgs::msg::Float32::SharedPtr vel);
  void setMoveCallback(const std_msgs::msg::Float32::SharedPtr move);

  void setStopServiceCallback(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                              const std::shared_ptr<std_srvs::srv::SetBool::Response> response);
  void setRunServiceCallback(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                             const std::shared_ptr<std_srvs::srv::SetBool::Response> response);
  void publishData();
};

}  // namespace rpp

#endif