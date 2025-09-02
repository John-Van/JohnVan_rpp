// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rpp_msgs:msg/SingleMotorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SINGLE_MOTOR_STATE__BUILDER_HPP_
#define RPP_MSGS__MSG__DETAIL__SINGLE_MOTOR_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rpp_msgs/msg/detail/single_motor_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rpp_msgs
{

namespace msg
{

namespace builder
{

class Init_SingleMotorState_fault
{
public:
  explicit Init_SingleMotorState_fault(::rpp_msgs::msg::SingleMotorState & msg)
  : msg_(msg)
  {}
  ::rpp_msgs::msg::SingleMotorState fault(::rpp_msgs::msg::SingleMotorState::_fault_type arg)
  {
    msg_.fault = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rpp_msgs::msg::SingleMotorState msg_;
};

class Init_SingleMotorState_tick
{
public:
  explicit Init_SingleMotorState_tick(::rpp_msgs::msg::SingleMotorState & msg)
  : msg_(msg)
  {}
  Init_SingleMotorState_fault tick(::rpp_msgs::msg::SingleMotorState::_tick_type arg)
  {
    msg_.tick = std::move(arg);
    return Init_SingleMotorState_fault(msg_);
  }

private:
  ::rpp_msgs::msg::SingleMotorState msg_;
};

class Init_SingleMotorState_rpm
{
public:
  explicit Init_SingleMotorState_rpm(::rpp_msgs::msg::SingleMotorState & msg)
  : msg_(msg)
  {}
  Init_SingleMotorState_tick rpm(::rpp_msgs::msg::SingleMotorState::_rpm_type arg)
  {
    msg_.rpm = std::move(arg);
    return Init_SingleMotorState_tick(msg_);
  }

private:
  ::rpp_msgs::msg::SingleMotorState msg_;
};

class Init_SingleMotorState_temperature
{
public:
  explicit Init_SingleMotorState_temperature(::rpp_msgs::msg::SingleMotorState & msg)
  : msg_(msg)
  {}
  Init_SingleMotorState_rpm temperature(::rpp_msgs::msg::SingleMotorState::_temperature_type arg)
  {
    msg_.temperature = std::move(arg);
    return Init_SingleMotorState_rpm(msg_);
  }

private:
  ::rpp_msgs::msg::SingleMotorState msg_;
};

class Init_SingleMotorState_current
{
public:
  explicit Init_SingleMotorState_current(::rpp_msgs::msg::SingleMotorState & msg)
  : msg_(msg)
  {}
  Init_SingleMotorState_temperature current(::rpp_msgs::msg::SingleMotorState::_current_type arg)
  {
    msg_.current = std::move(arg);
    return Init_SingleMotorState_temperature(msg_);
  }

private:
  ::rpp_msgs::msg::SingleMotorState msg_;
};

class Init_SingleMotorState_voltage
{
public:
  explicit Init_SingleMotorState_voltage(::rpp_msgs::msg::SingleMotorState & msg)
  : msg_(msg)
  {}
  Init_SingleMotorState_current voltage(::rpp_msgs::msg::SingleMotorState::_voltage_type arg)
  {
    msg_.voltage = std::move(arg);
    return Init_SingleMotorState_current(msg_);
  }

private:
  ::rpp_msgs::msg::SingleMotorState msg_;
};

class Init_SingleMotorState_omega
{
public:
  explicit Init_SingleMotorState_omega(::rpp_msgs::msg::SingleMotorState & msg)
  : msg_(msg)
  {}
  Init_SingleMotorState_voltage omega(::rpp_msgs::msg::SingleMotorState::_omega_type arg)
  {
    msg_.omega = std::move(arg);
    return Init_SingleMotorState_voltage(msg_);
  }

private:
  ::rpp_msgs::msg::SingleMotorState msg_;
};

class Init_SingleMotorState_theta
{
public:
  Init_SingleMotorState_theta()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SingleMotorState_omega theta(::rpp_msgs::msg::SingleMotorState::_theta_type arg)
  {
    msg_.theta = std::move(arg);
    return Init_SingleMotorState_omega(msg_);
  }

private:
  ::rpp_msgs::msg::SingleMotorState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rpp_msgs::msg::SingleMotorState>()
{
  return rpp_msgs::msg::builder::Init_SingleMotorState_theta();
}

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__SINGLE_MOTOR_STATE__BUILDER_HPP_
