// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from xf_voice_msgs:srv/Xf.idl
// generated code does not contain a copyright notice

#ifndef XF_VOICE_MSGS__SRV__DETAIL__XF__TRAITS_HPP_
#define XF_VOICE_MSGS__SRV__DETAIL__XF__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "xf_voice_msgs/srv/detail/xf__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace xf_voice_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const Xf_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: text
  {
    out << "text: ";
    rosidl_generator_traits::value_to_yaml(msg.text, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Xf_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: text
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "text: ";
    rosidl_generator_traits::value_to_yaml(msg.text, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Xf_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace xf_voice_msgs

namespace rosidl_generator_traits
{

[[deprecated("use xf_voice_msgs::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const xf_voice_msgs::srv::Xf_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  xf_voice_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xf_voice_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xf_voice_msgs::srv::Xf_Request & msg)
{
  return xf_voice_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xf_voice_msgs::srv::Xf_Request>()
{
  return "xf_voice_msgs::srv::Xf_Request";
}

template<>
inline const char * name<xf_voice_msgs::srv::Xf_Request>()
{
  return "xf_voice_msgs/srv/Xf_Request";
}

template<>
struct has_fixed_size<xf_voice_msgs::srv::Xf_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xf_voice_msgs::srv::Xf_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xf_voice_msgs::srv::Xf_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace xf_voice_msgs
{

namespace srv
{

inline void to_flow_style_yaml(
  const Xf_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Xf_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Xf_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace xf_voice_msgs

namespace rosidl_generator_traits
{

[[deprecated("use xf_voice_msgs::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const xf_voice_msgs::srv::Xf_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  xf_voice_msgs::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use xf_voice_msgs::srv::to_yaml() instead")]]
inline std::string to_yaml(const xf_voice_msgs::srv::Xf_Response & msg)
{
  return xf_voice_msgs::srv::to_yaml(msg);
}

template<>
inline const char * data_type<xf_voice_msgs::srv::Xf_Response>()
{
  return "xf_voice_msgs::srv::Xf_Response";
}

template<>
inline const char * name<xf_voice_msgs::srv::Xf_Response>()
{
  return "xf_voice_msgs/srv/Xf_Response";
}

template<>
struct has_fixed_size<xf_voice_msgs::srv::Xf_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<xf_voice_msgs::srv::Xf_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<xf_voice_msgs::srv::Xf_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<xf_voice_msgs::srv::Xf>()
{
  return "xf_voice_msgs::srv::Xf";
}

template<>
inline const char * name<xf_voice_msgs::srv::Xf>()
{
  return "xf_voice_msgs/srv/Xf";
}

template<>
struct has_fixed_size<xf_voice_msgs::srv::Xf>
  : std::integral_constant<
    bool,
    has_fixed_size<xf_voice_msgs::srv::Xf_Request>::value &&
    has_fixed_size<xf_voice_msgs::srv::Xf_Response>::value
  >
{
};

template<>
struct has_bounded_size<xf_voice_msgs::srv::Xf>
  : std::integral_constant<
    bool,
    has_bounded_size<xf_voice_msgs::srv::Xf_Request>::value &&
    has_bounded_size<xf_voice_msgs::srv::Xf_Response>::value
  >
{
};

template<>
struct is_service<xf_voice_msgs::srv::Xf>
  : std::true_type
{
};

template<>
struct is_service_request<xf_voice_msgs::srv::Xf_Request>
  : std::true_type
{
};

template<>
struct is_service_response<xf_voice_msgs::srv::Xf_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // XF_VOICE_MSGS__SRV__DETAIL__XF__TRAITS_HPP_
