// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rpp_msgs:msg/SetLight.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_LIGHT__TRAITS_HPP_
#define RPP_MSGS__MSG__DETAIL__SET_LIGHT__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rpp_msgs/msg/detail/set_light__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace rpp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const SetLight & msg,
  std::ostream & out)
{
  out << "{";
  // member: front_light
  {
    out << "front_light: ";
    rosidl_generator_traits::value_to_yaml(msg.front_light, out);
    out << ", ";
  }

  // member: rear_light
  {
    out << "rear_light: ";
    rosidl_generator_traits::value_to_yaml(msg.rear_light, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetLight & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: front_light
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "front_light: ";
    rosidl_generator_traits::value_to_yaml(msg.front_light, out);
    out << "\n";
  }

  // member: rear_light
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "rear_light: ";
    rosidl_generator_traits::value_to_yaml(msg.rear_light, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetLight & msg, bool use_flow_style = false)
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
  const rpp_msgs::msg::SetLight & msg,
  std::ostream & out, size_t indentation = 0)
{
  rpp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rpp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const rpp_msgs::msg::SetLight & msg)
{
  return rpp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<rpp_msgs::msg::SetLight>()
{
  return "rpp_msgs::msg::SetLight";
}

template<>
inline const char * name<rpp_msgs::msg::SetLight>()
{
  return "rpp_msgs/msg/SetLight";
}

template<>
struct has_fixed_size<rpp_msgs::msg::SetLight>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rpp_msgs::msg::SetLight>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rpp_msgs::msg::SetLight>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RPP_MSGS__MSG__DETAIL__SET_LIGHT__TRAITS_HPP_
