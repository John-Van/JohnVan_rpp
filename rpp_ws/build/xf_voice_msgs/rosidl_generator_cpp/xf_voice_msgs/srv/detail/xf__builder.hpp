// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from xf_voice_msgs:srv/Xf.idl
// generated code does not contain a copyright notice

#ifndef XF_VOICE_MSGS__SRV__DETAIL__XF__BUILDER_HPP_
#define XF_VOICE_MSGS__SRV__DETAIL__XF__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "xf_voice_msgs/srv/detail/xf__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace xf_voice_msgs
{

namespace srv
{

namespace builder
{

class Init_Xf_Request_text
{
public:
  Init_Xf_Request_text()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::xf_voice_msgs::srv::Xf_Request text(::xf_voice_msgs::srv::Xf_Request::_text_type arg)
  {
    msg_.text = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xf_voice_msgs::srv::Xf_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xf_voice_msgs::srv::Xf_Request>()
{
  return xf_voice_msgs::srv::builder::Init_Xf_Request_text();
}

}  // namespace xf_voice_msgs


namespace xf_voice_msgs
{

namespace srv
{

namespace builder
{

class Init_Xf_Response_message
{
public:
  explicit Init_Xf_Response_message(::xf_voice_msgs::srv::Xf_Response & msg)
  : msg_(msg)
  {}
  ::xf_voice_msgs::srv::Xf_Response message(::xf_voice_msgs::srv::Xf_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::xf_voice_msgs::srv::Xf_Response msg_;
};

class Init_Xf_Response_success
{
public:
  Init_Xf_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Xf_Response_message success(::xf_voice_msgs::srv::Xf_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_Xf_Response_message(msg_);
  }

private:
  ::xf_voice_msgs::srv::Xf_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::xf_voice_msgs::srv::Xf_Response>()
{
  return xf_voice_msgs::srv::builder::Init_Xf_Response_success();
}

}  // namespace xf_voice_msgs

#endif  // XF_VOICE_MSGS__SRV__DETAIL__XF__BUILDER_HPP_
