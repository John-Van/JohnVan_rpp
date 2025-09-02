// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rpp_msgs:msg/BatteryState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__BATTERY_STATE__BUILDER_HPP_
#define RPP_MSGS__MSG__DETAIL__BATTERY_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rpp_msgs/msg/detail/battery_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rpp_msgs
{

namespace msg
{

namespace builder
{

class Init_BatteryState_ts
{
public:
  explicit Init_BatteryState_ts(::rpp_msgs::msg::BatteryState & msg)
  : msg_(msg)
  {}
  ::rpp_msgs::msg::BatteryState ts(::rpp_msgs::msg::BatteryState::_ts_type arg)
  {
    msg_.ts = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rpp_msgs::msg::BatteryState msg_;
};

class Init_BatteryState_current
{
public:
  explicit Init_BatteryState_current(::rpp_msgs::msg::BatteryState & msg)
  : msg_(msg)
  {}
  Init_BatteryState_ts current(::rpp_msgs::msg::BatteryState::_current_type arg)
  {
    msg_.current = std::move(arg);
    return Init_BatteryState_ts(msg_);
  }

private:
  ::rpp_msgs::msg::BatteryState msg_;
};

class Init_BatteryState_voltage
{
public:
  explicit Init_BatteryState_voltage(::rpp_msgs::msg::BatteryState & msg)
  : msg_(msg)
  {}
  Init_BatteryState_current voltage(::rpp_msgs::msg::BatteryState::_voltage_type arg)
  {
    msg_.voltage = std::move(arg);
    return Init_BatteryState_current(msg_);
  }

private:
  ::rpp_msgs::msg::BatteryState msg_;
};

class Init_BatteryState_temperature
{
public:
  explicit Init_BatteryState_temperature(::rpp_msgs::msg::BatteryState & msg)
  : msg_(msg)
  {}
  Init_BatteryState_voltage temperature(::rpp_msgs::msg::BatteryState::_temperature_type arg)
  {
    msg_.temperature = std::move(arg);
    return Init_BatteryState_voltage(msg_);
  }

private:
  ::rpp_msgs::msg::BatteryState msg_;
};

class Init_BatteryState_percentage
{
public:
  Init_BatteryState_percentage()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_BatteryState_temperature percentage(::rpp_msgs::msg::BatteryState::_percentage_type arg)
  {
    msg_.percentage = std::move(arg);
    return Init_BatteryState_temperature(msg_);
  }

private:
  ::rpp_msgs::msg::BatteryState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rpp_msgs::msg::BatteryState>()
{
  return rpp_msgs::msg::builder::Init_BatteryState_percentage();
}

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__BATTERY_STATE__BUILDER_HPP_
