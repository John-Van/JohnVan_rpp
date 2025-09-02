// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rpp_msgs:msg/SetDevice.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_DEVICE__TRAITS_HPP_
#define RPP_MSGS__MSG__DETAIL__SET_DEVICE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rpp_msgs/msg/detail/set_device__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace rpp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const SetDevice & msg,
  std::ostream & out)
{
  out << "{";
  // member: control_mode
  {
    out << "control_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.control_mode, out);
    out << ", ";
  }

  // member: power_mode
  {
    out << "power_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.power_mode, out);
    out << ", ";
  }

  // member: motor_mode
  {
    out << "motor_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.motor_mode, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetDevice & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: control_mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "control_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.control_mode, out);
    out << "\n";
  }

  // member: power_mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "power_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.power_mode, out);
    out << "\n";
  }

  // member: motor_mode
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "motor_mode: ";
    rosidl_generator_traits::value_to_yaml(msg.motor_mode, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetDevice & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace rpp_msgs

namespace rosidl_generator_traits
{

[[deprecated("use rpp_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const rpp_msgs::msg::SetDevice & msg,
  std::ostream & out, size_t indentation = 0)
{
  rpp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rpp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const rpp_msgs::msg::SetDevice & msg)
{
  return rpp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<rpp_msgs::msg::SetDevice>()
{
  return "rpp_msgs::msg::SetDevice";
}

template<>
inline const char * name<rpp_msgs::msg::SetDevice>()
{
  return "rpp_msgs/msg/SetDevice";
}

template<>
struct has_fixed_size<rpp_msgs::msg::SetDevice>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rpp_msgs::msg::SetDevice>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rpp_msgs::msg::SetDevice>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RPP_MSGS__MSG__DETAIL__SET_DEVICE__TRAITS_HPP_
