// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rpp_msgs:msg/SetErrorMask.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_ERROR_MASK__BUILDER_HPP_
#define RPP_MSGS__MSG__DETAIL__SET_ERROR_MASK__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rpp_msgs/msg/detail/set_error_mask__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rpp_msgs
{

namespace msg
{

namespace builder
{

class Init_SetErrorMask_bump_mask
{
public:
  explicit Init_SetErrorMask_bump_mask(::rpp_msgs::msg::SetErrorMask & msg)
  : msg_(msg)
  {}
  ::rpp_msgs::msg::SetErrorMask bump_mask(::rpp_msgs::msg::SetErrorMask::_bump_mask_type arg)
  {
    msg_.bump_mask = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rpp_msgs::msg::SetErrorMask msg_;
};

class Init_SetErrorMask_motor_mask
{
public:
  Init_SetErrorMask_motor_mask()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetErrorMask_bump_mask motor_mask(::rpp_msgs::msg::SetErrorMask::_motor_mask_type arg)
  {
    msg_.motor_mask = std::move(arg);
    return Init_SetErrorMask_bump_mask(msg_);
  }

private:
  ::rpp_msgs::msg::SetErrorMask msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rpp_msgs::msg::SetErrorMask>()
{
  return rpp_msgs::msg::builder::Init_SetErrorMask_motor_mask();
}

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__SET_ERROR_MASK__BUILDER_HPP_
