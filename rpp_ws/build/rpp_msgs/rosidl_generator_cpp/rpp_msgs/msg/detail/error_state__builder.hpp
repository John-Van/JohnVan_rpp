// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rpp_msgs:msg/ErrorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__ERROR_STATE__BUILDER_HPP_
#define RPP_MSGS__MSG__DETAIL__ERROR_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rpp_msgs/msg/detail/error_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rpp_msgs
{

namespace msg
{

namespace builder
{

class Init_ErrorState_ts
{
public:
  explicit Init_ErrorState_ts(::rpp_msgs::msg::ErrorState & msg)
  : msg_(msg)
  {}
  ::rpp_msgs::msg::ErrorState ts(::rpp_msgs::msg::ErrorState::_ts_type arg)
  {
    msg_.ts = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rpp_msgs::msg::ErrorState msg_;
};

class Init_ErrorState_communication_error
{
public:
  explicit Init_ErrorState_communication_error(::rpp_msgs::msg::ErrorState & msg)
  : msg_(msg)
  {}
  Init_ErrorState_ts communication_error(::rpp_msgs::msg::ErrorState::_communication_error_type arg)
  {
    msg_.communication_error = std::move(arg);
    return Init_ErrorState_ts(msg_);
  }

private:
  ::rpp_msgs::msg::ErrorState msg_;
};

class Init_ErrorState_bump_error
{
public:
  explicit Init_ErrorState_bump_error(::rpp_msgs::msg::ErrorState & msg)
  : msg_(msg)
  {}
  Init_ErrorState_communication_error bump_error(::rpp_msgs::msg::ErrorState::_bump_error_type arg)
  {
    msg_.bump_error = std::move(arg);
    return Init_ErrorState_communication_error(msg_);
  }

private:
  ::rpp_msgs::msg::ErrorState msg_;
};

class Init_ErrorState_turn_motor_error
{
public:
  explicit Init_ErrorState_turn_motor_error(::rpp_msgs::msg::ErrorState & msg)
  : msg_(msg)
  {}
  Init_ErrorState_bump_error turn_motor_error(::rpp_msgs::msg::ErrorState::_turn_motor_error_type arg)
  {
    msg_.turn_motor_error = std::move(arg);
    return Init_ErrorState_bump_error(msg_);
  }

private:
  ::rpp_msgs::msg::ErrorState msg_;
};

class Init_ErrorState_motor_error
{
public:
  Init_ErrorState_motor_error()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ErrorState_turn_motor_error motor_error(::rpp_msgs::msg::ErrorState::_motor_error_type arg)
  {
    msg_.motor_error = std::move(arg);
    return Init_ErrorState_turn_motor_error(msg_);
  }

private:
  ::rpp_msgs::msg::ErrorState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rpp_msgs::msg::ErrorState>()
{
  return rpp_msgs::msg::builder::Init_ErrorState_motor_error();
}

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__ERROR_STATE__BUILDER_HPP_
