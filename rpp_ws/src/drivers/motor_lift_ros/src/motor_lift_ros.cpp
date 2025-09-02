#include "motor_lift_ros/motor_lift_ros.h"
#include <functional>

namespace rpp
{

Syntron_Servo_ROS::Syntron_Servo_ROS() : Node("motor_lift_ros_node")
{
  // 声明节点参数与默认值
  this->declare_parameter<std::string>("can_port", "can0");
  this->declare_parameter<char>("can_motor_id", 0x02);
  this->declare_parameter<std::string>("lift_joint", "connector_joint");

  this->declare_parameter<float>("high_limit", 0.0);
  this->declare_parameter<float>("low_limit", -0.46);
  this->declare_parameter<float>("velocity", 0.028);

  can_port_ = this->get_parameter("can_port").get_value<std::string>();
  can_id_ = this->get_parameter("can_motor_id").get_value<char>();
  lift_joint_ = this->get_parameter("lift_joint").get_value<std::string>();

  //   使用指针，避免需要提前初始化
  rpp_driver_ = new Syntron_Servo(can_port_, can_id_);
  // 单位转换
  rpp_driver_->setHighLimit(this->get_parameter("high_limit").get_value<float>() * 100.0);
  rpp_driver_->setLowLimit(this->get_parameter("low_limit").get_value<float>() * 100.0);
  velocity_set_ = this->get_parameter("velocity").get_value<float>();
  rpp_driver_->setVelocity(velocity_set_);

  rpp_driver_->start();

  set_position_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      "/motor_lift/set_position", 5, std::bind(&Syntron_Servo_ROS::setPositionCallback, this, std::placeholders::_1));
  set_velocity_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      "/motor_lift/set_velocity", 5, std::bind(&Syntron_Servo_ROS::setVelocityCallback, this, std::placeholders::_1));
  set_move_sub_ = this->create_subscription<std_msgs::msg::Float32>(
      "/motor_lift/set_move", 5, std::bind(&Syntron_Servo_ROS::setMoveCallback, this, std::placeholders::_1));

  set_stop_srv_ = this->create_service<std_srvs::srv::SetBool>(
      "/motor_lift/set_emergency_stop",
      std::bind(&Syntron_Servo_ROS::setStopServiceCallback, this, std::placeholders::_1, std::placeholders::_2));
  set_run_srv_ = this->create_service<std_srvs::srv::SetBool>(
      "/motor_lift/set_enable",
      std::bind(&Syntron_Servo_ROS::setRunServiceCallback, this, std::placeholders::_1, std::placeholders::_2));

  position_pub_ = this->create_publisher<std_msgs::msg::Float32>("motor_lift/position", 10);
  joint_pub_ = this->create_publisher<sensor_msgs::msg::JointState>("joint_states", 100);

  publish_data_timer_ = this->create_wall_timer(std::chrono::milliseconds(10),
                                                std::bind(&Syntron_Servo_ROS::publishData, this));  // 100hz

  // 输出电机初始化的参数信息
  RCLCPP_INFO(this->get_logger(), "Lift Motor initialization parameters:");
  RCLCPP_INFO(this->get_logger(), "CAN Port: %s", can_port_.c_str());
  RCLCPP_INFO(this->get_logger(), "CAN Motor ID: 0x%02X", static_cast<unsigned int>(can_id_));
  RCLCPP_INFO(this->get_logger(), "Lift Joint: %s", lift_joint_.c_str());
  RCLCPP_INFO(this->get_logger(), "High Limit: %.2f", this->get_parameter("high_limit").get_value<float>());
  RCLCPP_INFO(this->get_logger(), "Low Limit: %.2f", this->get_parameter("low_limit").get_value<float>());
  RCLCPP_INFO(this->get_logger(), "Velocity: %.5f m/s", this->get_parameter("velocity").get_value<float>());
}

Syntron_Servo_ROS::~Syntron_Servo_ROS()
{
  if (rpp_driver_ != nullptr)
  {
    delete rpp_driver_;
    rpp_driver_ = nullptr;
  }
  // 显式销毁节点，确保资源正确释放
  rclcpp::shutdown();
}

void Syntron_Servo_ROS::setPositionCallback(const std_msgs::msg::Float32::SharedPtr cmd)
{
  if (rpp_driver_ == nullptr)
  {
    RCLCPP_WARN(this->get_logger(), "Lift Motor is not initialized.");
    return;
  }
  if (rpp_driver_->running == false)
  {
    RCLCPP_WARN(this->get_logger(), "Motor is not running, failed to set its position.");
    return;
  }
  // 米转为厘米
  float pos = cmd->data * 100.0;
  rpp_driver_->setPosition(pos);
  if (pos > rpp_driver_->highlimit || pos < rpp_driver_->lowlimit)
  {
    RCLCPP_ERROR(this->get_logger(), "Set position out of limit, motor move to the nearest limit.");
  }
  else
  {
    RCLCPP_INFO(this->get_logger(), "Set position: %.2f (m)", cmd->data);
  }
}

void Syntron_Servo_ROS::setVelocityCallback(const std_msgs::msg::Float32::SharedPtr vel)
{
  if (rpp_driver_ == nullptr)
  {
    RCLCPP_WARN(this->get_logger(), "Lift Motor is not initialized.");
    return;
  }
  
  // 现在底层setVelocity直接接受m/s，无需转换
  float linear_velocity = vel->data;
  
  // 检查速度范围 - 转换为rpm来检查限制
  float mm_per_min = linear_velocity * 1000.0 * 60.0;
  float rpm_equivalent = mm_per_min * 6.0 / 5.0;
  
  if (rpm_equivalent > SyntronCanCode::VELOCITY_MAX)
  {
    float default_v = this->get_parameter("velocity").get_value<float>();
    RCLCPP_ERROR(this->get_logger(), "The given velocity may cause danger, set to default value: %.5f m/s.", default_v);
    velocity_set_ = default_v;
    rpp_driver_->setVelocity(velocity_set_);
    return;
  }
  else if (rpm_equivalent < SyntronCanCode::VELOCITY_MIN)
  {
    float default_v = this->get_parameter("velocity").get_value<float>();
    RCLCPP_ERROR(this->get_logger(), "The given velocity is too small, set to default value: %.5f m/s.", default_v);
    velocity_set_ = default_v;
    rpp_driver_->setVelocity(velocity_set_);
    return;
  }
  velocity_set_ = linear_velocity;  // 更新成员变量
  // 设置速度
  rpp_driver_->setVelocity(velocity_set_);
  RCLCPP_INFO(this->get_logger(), "Set velocity: %.5f m/s", velocity_set_);
}

void Syntron_Servo_ROS::setMoveCallback(const std_msgs::msg::Float32::SharedPtr move)
{
  float move_threshold = 0.5;
  // 现在velocity是m/s，计算每次移动的步长（单位：cm）
  // velocity(m/s) * 时间间隔(s) * 倍数 * 100(m转cm)
  float move_step = velocity_set_ / 0.03 * 2.0;  // 每次移动的步长，根据速度计算
  if (rpp_driver_ == nullptr)
  {
    RCLCPP_WARN(this->get_logger(), "Lift Motor is not initialized.");
    return;
  }
  if (move->data > move_threshold)
  {
    RCLCPP_INFO(this->get_logger(), "Lift Motor move up.");
    rpp_driver_->setPosition(rpp_driver_->position + move_step);
    return;
  }
  else if (move->data < -move_threshold)
  {
    RCLCPP_INFO(this->get_logger(), "Lift Motor move down.");
    rpp_driver_->setPosition(rpp_driver_->position - move_step);
    return;
  }
}

void Syntron_Servo_ROS::setRunServiceCallback(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                                              const std::shared_ptr<std_srvs::srv::SetBool::Response> response)
{
  if (rpp_driver_ == nullptr)
  {
    response->success = false;
    response->message = "Lift Motor is not initialized.";
    return;
  }
  if (request->data)
  {
    rpp_driver_->start();
    response->success = true;
    response->message = "Successfully started motor.";
    RCLCPP_INFO(this->get_logger(), "Successfully start motor.");
  }
  else
  {
    rpp_driver_->stop();
    response->success = true;
    response->message = "Successfully stoped motor.";
    RCLCPP_INFO(this->get_logger(), "Successfully stoped motor.");
  }
}

void Syntron_Servo_ROS::setStopServiceCallback(const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
                                               const std::shared_ptr<std_srvs::srv::SetBool::Response> response)
{
  if (rpp_driver_ == nullptr)
  {
    response->success = false;
    response->message = "Lift Motor is not initialized.";
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

// 发布信息的总函数
void Syntron_Servo_ROS::publishData()
{
  if (rpp_driver_ == nullptr)
    return;

  if (rpp_driver_->inplace && !show_inplace_)
  {
    RCLCPP_INFO(this->get_logger(), "Motor is in place. Current position: %.2f",
                static_cast<double>(rpp_driver_->position));
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
    // 以米为单位进行发布
    msg.data = std::round(rpp_driver_->position * 100.0) / 10000.0;
    position_pub_->publish(msg);
  }

  if (joint_pub_->get_subscription_count() != 0)
  {
    sensor_msgs::msg::JointState msg;
    msg.header.stamp = this->get_clock()->now();
    msg.name.push_back(lift_joint_);
    // 对 position 进行四舍五入并保留两位小数，由厘米换算到米
    float rounded_position = std::round(rpp_driver_->position * 100.0) / 10000.0;
    msg.position.push_back(rounded_position);
    joint_pub_->publish(msg);
  }
}
}  // namespace rpp