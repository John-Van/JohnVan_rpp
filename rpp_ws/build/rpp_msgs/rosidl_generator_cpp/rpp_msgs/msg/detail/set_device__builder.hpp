// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rpp_msgs:msg/SetDevice.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_DEVICE__BUILDER_HPP_
#define RPP_MSGS__MSG__DETAIL__SET_DEVICE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rpp_msgs/msg/detail/set_device__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rpp_msgs
{

namespace msg
{

namespace builder
{

class Init_SetDevice_motor_mode
{
public:
  explicit Init_SetDevice_motor_mode(::rpp_msgs::msg::SetDevice & msg)
  : msg_(msg)
  {}
  ::rpp_msgs::msg::SetDevice motor_mode(::rpp_msgs::msg::SetDevice::_motor_mode_type arg)
  {
    msg_.motor_mode = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rpp_msgs::msg::SetDevice msg_;
};

class Init_SetDevice_power_mode
{
public:
  explicit Init_SetDevice_power_mode(::rpp_msgs::msg::SetDevice & msg)
  : msg_(msg)
  {}
  Init_SetDevice_motor_mode power_mode(::rpp_msgs::msg::SetDevice::_power_mode_type arg)
  {
    msg_.power_mode = std::move(arg);
    return Init_SetDevice_motor_mode(msg_);
  }

private:
  ::rpp_msgs::msg::SetDevice msg_;
};

class Init_SetDevice_control_mode
{
public:
  Init_SetDevice_control_mode()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetDevice_power_mode control_mode(::rpp_msgs::msg::SetDevice::_control_mode_type arg)
  {
    msg_.control_mode = std::move(arg);
    return Init_SetDevice_power_mode(msg_);
  }

private:
  ::rpp_msgs::msg::SetDevice msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rpp_msgs::msg::SetDevice>()
{
  return rpp_msgs::msg::builder::Init_SetDevice_control_mode();
}

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__SET_DEVICE__BUILDER_HPP_
