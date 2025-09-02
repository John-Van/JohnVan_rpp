// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rpp_msgs:msg/SetLight.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_LIGHT__BUILDER_HPP_
#define RPP_MSGS__MSG__DETAIL__SET_LIGHT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rpp_msgs/msg/detail/set_light__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rpp_msgs
{

namespace msg
{

namespace builder
{

class Init_SetLight_rear_light
{
public:
  explicit Init_SetLight_rear_light(::rpp_msgs::msg::SetLight & msg)
  : msg_(msg)
  {}
  ::rpp_msgs::msg::SetLight rear_light(::rpp_msgs::msg::SetLight::_rear_light_type arg)
  {
    msg_.rear_light = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rpp_msgs::msg::SetLight msg_;
};

class Init_SetLight_front_light
{
public:
  Init_SetLight_front_light()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetLight_rear_light front_light(::rpp_msgs::msg::SetLight::_front_light_type arg)
  {
    msg_.front_light = std::move(arg);
    return Init_SetLight_rear_light(msg_);
  }

private:
  ::rpp_msgs::msg::SetLight msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rpp_msgs::msg::SetLight>()
{
  return rpp_msgs::msg::builder::Init_SetLight_front_light();
}

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__SET_LIGHT__BUILDER_HPP_
