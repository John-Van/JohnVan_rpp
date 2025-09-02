// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rpp_msgs:msg/LightState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__LIGHT_STATE__BUILDER_HPP_
#define RPP_MSGS__MSG__DETAIL__LIGHT_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rpp_msgs/msg/detail/light_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rpp_msgs
{

namespace msg
{

namespace builder
{

class Init_LightState_ts
{
public:
  explicit Init_LightState_ts(::rpp_msgs::msg::LightState & msg)
  : msg_(msg)
  {}
  ::rpp_msgs::msg::LightState ts(::rpp_msgs::msg::LightState::_ts_type arg)
  {
    msg_.ts = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rpp_msgs::msg::LightState msg_;
};

class Init_LightState_rear_light
{
public:
  explicit Init_LightState_rear_light(::rpp_msgs::msg::LightState & msg)
  : msg_(msg)
  {}
  Init_LightState_ts rear_light(::rpp_msgs::msg::LightState::_rear_light_type arg)
  {
    msg_.rear_light = std::move(arg);
    return Init_LightState_ts(msg_);
  }

private:
  ::rpp_msgs::msg::LightState msg_;
};

class Init_LightState_front_light
{
public:
  Init_LightState_front_light()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LightState_rear_light front_light(::rpp_msgs::msg::LightState::_front_light_type arg)
  {
    msg_.front_light = std::move(arg);
    return Init_LightState_rear_light(msg_);
  }

private:
  ::rpp_msgs::msg::LightState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rpp_msgs::msg::LightState>()
{
  return rpp_msgs::msg::builder::Init_LightState_front_light();
}

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__LIGHT_STATE__BUILDER_HPP_
