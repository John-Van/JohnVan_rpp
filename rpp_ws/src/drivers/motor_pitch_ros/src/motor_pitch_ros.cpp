#include "motor_pitch_ros/motor_pitch_ros.h"

namespace rpp
{

TI5CRA_Servo_ROS::TI5CRA_Servo_ROS() : Node("motor_pitch_ros_node")
{
  // 声明节点参数与默认值
  this->declare_parameter<std::string>("can_port", "can0");
  this->declare_parameter<char>("can_motor_id", 0x01);
  this->declare_parameter<std::string>("joint_name", "trunk_2_joint");
  this->declare_parameter<float>("high_limit", 90);
  this->declare_parameter<float>("low_limit", 0);
  this->declare_parameter<float>("velocity", 2);

  can_port_ = this->get_parameter("can_port").get_value<std::string>();
  can_id_ = this->get_parameter("can_motor_id").get_value<char>();
  joint_name_ = this->get_parameter("joint_name").get_value<std::string>();

  //   使用指针，避免需要提前初始化
  rpp_driver_ = new Ti5CRA_Servo(can_port_, can_id_);
  rpp_driver_->start();

  rpp_driver_->setHighLimit(this->get_parameter("high_limit").get_value<float>());
  rpp_driver_->setLowLimit(this->get_parameter("low_limit").get_value<float>());
  // 设置默认速度
  velocity_set_ = this->get_parameter("velocity").get_value<float>();
  rpp_driver_->setVelocity(velocity_set_);

  set_position_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      "/motor_pitch/set_position", 5, std::bind(&TI5CRA_Servo_ROS::setPositionCallback, this, std::placeholders::_1));
  set_velocity_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      "/motor_pitch/set_velocity", 5, std::bind(&TI5CRA_Servo_ROS::setVelocityCallback, this, std::placeholders::_1));
  set_move_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      "/motor_pitch/set_move", 5, std::bind(&TI5CRA_Servo_ROS::setMoveCallback, this, std::placeholders::_1));

  set_stop_srv_ = this->create_service<std_srvs::srv::SetBool>(
      "/motor_pitch/set_emergency_stop",
      std::bind(&TI5CRA_Servo_ROS::setStopServiceCallback, this, std::placeholders::_1, std::placeholders::_2));
  set_run_srv_ = this->create_service<std_srvs::srv::SetBool>(
      "/motor_pitch/set_enable",
      std::bind(&TI5CRA_Servo_ROS::setRunServiceCallback, this, std::placeholders::_1, std::placeholders::_2));

  position_pub_ = this->create_publisher<std_msgs::msg::Float32>("motor_pitch/position", 10);
  joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>("joint_states", 10);

  publish_data_timer_ =
      this->create_wall_timer(std::chrono::milliseconds(10), std::bind(&TI5CRA_Servo_ROS::publishData, this));  // 100hz

  // 输出电机初始化的参数信息
  RCLCPP_INFO(this->get_logger(), "Pitch Motor initialization parameters:");
  RCLCPP_INFO(this->get_logger(), "CAN Port: %s", can_port_.c_str());
  RCLCPP_INFO(this->get_logger(), "CAN Motor ID: 0x%02X", static_cast<unsigned int>(can_id_));
  RCLCPP_INFO(this->get_logger(), "Pitch Joint: %s", joint_name_.c_str());
  RCLCPP_INFO(this->get_logger(), "High Limit: %.2f", this->get_parameter("high_limit").get_value<float>());
  RCLCPP_INFO(this->get_logger(), "Low Limit: %.2f", this->get_parameter("low_limit").get_value<float>());
  RCLCPP_INFO(this->get_logger(), "Velocity: %.2f", this->get_parameter("velocity").get_value<float>());
}

TI5CRA_Servo_ROS::~TI5CRA_Servo_ROS()
{
  if (rpp_driver_ != nullptr)
  {
    delete rpp_driver_;
    rpp_driver_ = nullptr;
  }
  // 显式销毁节点，确保资源正确释放
  rclcpp::shutdown();
}

void TI5CRA_Servo_ROS::setPositionCallback(const std_msgs::msg::Float32::SharedPtr cmd)
{
  if (rpp_driver_ == nullptr)
  {
    RCLCPP_WARN(this->get_logger(), "Pitch_Servo is not initialized.");
    return;
  }
  if (rpp_driver_->running == false)
  {
    RCLCPP_WARN(this->get_logger(), "Motor is not running, failed to set its position.");
    return;
  }
  // 将弧度转换为角度
  const float rad_to_deg = 180.0f / M_PI;
  float angle = cmd->data * rad_to_deg;
  // 使用转换后的角度值设置电机位置
  rpp_driver_->setPosition(angle);
  if (angle > rpp_driver_->highlimit || angle < rpp_driver_->lowlimit)
  {
    RCLCPP_ERROR(this->get_logger(), "Set position out of limit, motor move to the nearest limit.");
  }
  else
  {
    RCLCPP_INFO(this->get_logger(), "Set position: %.2f (rad), %.2f (degree)", cmd->data, angle);
  }
}

void TI5CRA_Servo_ROS::setVelocityCallback(const std_msgs::msg::Float32::SharedPtr vel)
{
  if (rpp_driver_ == nullptr)
  {
    RCLCPP_WARN(this->get_logger(), "Pitch_Servo is not initialized.");
    return;
  }
  if (vel->data > Ti5CRAFunctionCode::VELOCITY_MAX)
  {
    float v = this->get_parameter("velocity").get_value<float>();
    RCLCPP_ERROR(this->get_logger(), "The given velocity may cause danger, set to default value: %.2f.", v);
    velocity_set_ = v;  // 更新成员变量
    rpp_driver_->setVelocity(velocity_set_);
    return;
  }
  else if (vel->data < Ti5CRAFunctionCode::VELOCITY_MIN)
  {
    float v = this->get_parameter("velocity").get_value<float>();
    RCLCPP_ERROR(this->get_logger(), "The given velocity is negative, set to default value: %.2f.", v);
    velocity_set_ = v;
    rpp_driver_->setVelocity(velocity_set_);
    return;
  }
  velocity_set_ = vel->data;
  rpp_driver_->setVelocity(velocity_set_);
  RCLCPP_INFO(this->get_logger(), "Set velocity: %.2f", velocity_set_);
}

void TI5CRA_Servo_ROS::setMoveCallback(const std_msgs::msg::Float32::SharedPtr move)
{
  float move_threshold = 0.5;
  // float move_step = velocity_set_ / 2.0 * 5.0;  // 每次移动的步长，根据速度计算
  // 俯仰电机步长固定
  float move_step = 5.0;
  if (rpp_driver_ == nullptr)
  {
    RCLCPP_WARN(this->get_logger(), "Pitch Motor is not initialized.");
    return;
  }
  if (move->data > move_threshold)
  {
    RCLCPP_INFO(this->get_logger(), "Pitch Motor move down.");
    rpp_driver_->setPosition(rpp_driver_->position + move_step);
    return;
  }
  else if (move->data < -move_threshold)
  {
    RCLCPP_INFO(this->get_logger(), "Pitch Motor move up.");
    rpp_driver_->setPosition(rpp_driver_->position - move_step);
    return;
  }
}

void TI5CRA_Servo_ROS::setStopServiceCallback(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                                              const std::shared_ptr<std_srvs::srv::SetBool::Response> response)
{
  if (rpp_driver_ == nullptr)
  {
    response->success = false;
    response->message = "Pitch_Servo is not initialized.";
    return;
  }

  if (request->data)
  {
    rpp_driver_->setStop();
    rpp_driver_->stop();
    response->success = true;
    response->message = "Successfully set stop.";
    RCLCPP_INFO(this->get_logger(), "Successfully set stop.");
  }
  else
  {
    response->success = false;
    response->message = "Request data is false, no action taken.";
    RCLCPP_INFO(this->get_logger(), "Request data is false, no action taken.");
  }
}

void TI5CRA_Servo_ROS::setRunServiceCallback(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                                             const std::shared_ptr<std_srvs::srv::SetBool::Response> response)
{
  if (rpp_driver_ == nullptr)
  {
    response->success = false;
    response->message = "Ti5CRA_Servo is not initialized.";
    return;
  }

  if (request->data)
  {
    rpp_driver_->start();
    response->success = true;
    response->message = "Motor set start.";
    RCLCPP_INFO(this->get_logger(), "Motor set start.");
  }
  else
  {
    rpp_driver_->stop();
    response->success = true;
    response->message = "Motor set stop.";
    RCLCPP_INFO(this->get_logger(), "Motor set stop.");
  }
}

// 发布信息的总函数，每10ms进入一次
void TI5CRA_Servo_ROS::publishData()
{
  if (rpp_driver_ == nullptr)
    return;

  // 使用 round 函数进行四舍五入
  float deg = std::round(rpp_driver_->position * 100) / 100.0;
  float rad = (deg / 180) * 3.1416;
  rad = std::round(rad * 100) / 100.0;

  if (rpp_driver_->inplace && !show_inplace_)
  {
    RCLCPP_INFO(this->get_logger(), "Motor is in place. Current position: %.2f (rad), %.2f (degree)", rad, deg);
    show_inplace_ = true;
  }
  if (!rpp_driver_->inplace && show_inplace_)
  {
    RCLCPP_INFO(this->get_logger(), "Motor starts moving");
    show_inplace_ = false;
  }

  if (position_pub_->get_subscription_count() != 0)
  {
    std_msgs::msg::Float32 msg;
    msg.data = rad;
    position_pub_->publish(msg);
  }

  if (joint_pub_->get_subscription_count() != 0)
  {
    sensor_msgs::msg::JointState msg;
    msg.header.stamp = this->get_clock()->now();
    msg.name.push_back(joint_name_);
    msg.position.push_back(rad);
    joint_pub_->publish(msg);
  }
}
}  // namespace rpp