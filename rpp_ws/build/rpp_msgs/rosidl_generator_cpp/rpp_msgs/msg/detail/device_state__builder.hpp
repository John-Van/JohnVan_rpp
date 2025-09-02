// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rpp_msgs:msg/DeviceState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__DEVICE_STATE__BUILDER_HPP_
#define RPP_MSGS__MSG__DETAIL__DEVICE_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rpp_msgs/msg/detail/device_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rpp_msgs
{

namespace msg
{

namespace builder
{

class Init_DeviceState_ts
{
public:
  explicit Init_DeviceState_ts(::rpp_msgs::msg::DeviceState & msg)
  : msg_(msg)
  {}
  ::rpp_msgs::msg::DeviceState ts(::rpp_msgs::msg::DeviceState::_ts_type arg)
  {
    msg_.ts = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rpp_msgs::msg::DeviceState msg_;
};

class Init_DeviceState_e_stop
{
public:
  explicit Init_DeviceState_e_stop(::rpp_msgs::msg::DeviceState & msg)
  : msg_(msg)
  {}
  Init_DeviceState_ts e_stop(::rpp_msgs::msg::DeviceState::_e_stop_type arg)
  {
    msg_.e_stop = std::move(arg);
    return Init_DeviceState_ts(msg_);
  }

private:
  ::rpp_msgs::msg::DeviceState msg_;
};

class Init_DeviceState_motor_mode
{
public:
  explicit Init_DeviceState_motor_mode(::rpp_msgs::msg::DeviceState & msg)
  : msg_(msg)
  {}
  Init_DeviceState_e_stop motor_mode(::rpp_msgs::msg::DeviceState::_motor_mode_type arg)
  {
    msg_.motor_mode = std::move(arg);
    return Init_DeviceState_e_stop(msg_);
  }

private:
  ::rpp_msgs::msg::DeviceState msg_;
};

class Init_DeviceState_power_mode
{
public:
  explicit Init_DeviceState_power_mode(::rpp_msgs::msg::DeviceState & msg)
  : msg_(msg)
  {}
  Init_DeviceState_motor_mode power_mode(::rpp_msgs::msg::DeviceState::_power_mode_type arg)
  {
    msg_.power_mode = std::move(arg);
    return Init_DeviceState_motor_mode(msg_);
  }

private:
  ::rpp_msgs::msg::DeviceState msg_;
};

class Init_DeviceState_control_mode
{
public:
  Init_DeviceState_control_mode()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_DeviceState_power_mode control_mode(::rpp_msgs::msg::DeviceState::_control_mode_type arg)
  {
    msg_.control_mode = std::move(arg);
    return Init_DeviceState_power_mode(msg_);
  }

private:
  ::rpp_msgs::msg::DeviceState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rpp_msgs::msg::DeviceState>()
{
  return rpp_msgs::msg::builder::Init_DeviceState_control_mode();
}

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__DEVICE_STATE__BUILDER_HPP_
