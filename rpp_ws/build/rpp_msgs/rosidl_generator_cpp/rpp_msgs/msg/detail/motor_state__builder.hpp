// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from rpp_msgs:msg/MotorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__MOTOR_STATE__BUILDER_HPP_
#define RPP_MSGS__MSG__DETAIL__MOTOR_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "rpp_msgs/msg/detail/motor_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace rpp_msgs
{

namespace msg
{

namespace builder
{

class Init_MotorState_ts
{
public:
  explicit Init_MotorState_ts(::rpp_msgs::msg::MotorState & msg)
  : msg_(msg)
  {}
  ::rpp_msgs::msg::MotorState ts(::rpp_msgs::msg::MotorState::_ts_type arg)
  {
    msg_.ts = std::move(arg);
    return std::move(msg_);
  }

private:
  ::rpp_msgs::msg::MotorState msg_;
};

class Init_MotorState_motor_states
{
public:
  Init_MotorState_motor_states()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MotorState_ts motor_states(::rpp_msgs::msg::MotorState::_motor_states_type arg)
  {
    msg_.motor_states = std::move(arg);
    return Init_MotorState_ts(msg_);
  }

private:
  ::rpp_msgs::msg::MotorState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::rpp_msgs::msg::MotorState>()
{
  return rpp_msgs::msg::builder::Init_MotorState_motor_states();
}

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__MOTOR_STATE__BUILDER_HPP_
