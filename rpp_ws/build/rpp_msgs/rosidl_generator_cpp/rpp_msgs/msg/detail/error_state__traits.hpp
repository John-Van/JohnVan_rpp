// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rpp_msgs:msg/ErrorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__ERROR_STATE__TRAITS_HPP_
#define RPP_MSGS__MSG__DETAIL__ERROR_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rpp_msgs/msg/detail/error_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace rpp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const ErrorState & msg,
  std::ostream & out)
{
  out << "{";
  // member: motor_error
  {
    out << "motor_error: ";
    rosidl_generator_traits::value_to_yaml(msg.motor_error, out);
    out << ", ";
  }

  // member: turn_motor_error
  {
    out << "turn_motor_error: ";
    rosidl_generator_traits::value_to_yaml(msg.turn_motor_error, out);
    out << ", ";
  }

  // member: bump_error
  {
    out << "bump_error: ";
    rosidl_generator_traits::value_to_yaml(msg.bump_error, out);
    out << ", ";
  }

  // member: communication_error
  {
    out << "communication_error: ";
    rosidl_generator_traits::value_to_yaml(msg.communication_error, out);
    out << ", ";
  }

  // member: ts
  {
    out << "ts: ";
    rosidl_generator_traits::value_to_yaml(msg.ts, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ErrorState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: motor_error
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "motor_error: ";
    rosidl_generator_traits::value_to_yaml(msg.motor_error, out);
    out << "\n";
  }

  // member: turn_motor_error
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "turn_motor_error: ";
    rosidl_generator_traits::value_to_yaml(msg.turn_motor_error, out);
    out << "\n";
  }

  // member: bump_error
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "bump_error: ";
    rosidl_generator_traits::value_to_yaml(msg.bump_error, out);
    out << "\n";
  }

  // member: communication_error
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "communication_error: ";
    rosidl_generator_traits::value_to_yaml(msg.communication_error, out);
    out << "\n";
  }

  // member: ts
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "ts: ";
    rosidl_generator_traits::value_to_yaml(msg.ts, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ErrorState & msg, bool use_flow_style = false)
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
  const rpp_msgs::msg::ErrorState & msg,
  std::ostream & out, size_t indentation = 0)
{
  rpp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rpp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const rpp_msgs::msg::ErrorState & msg)
{
  return rpp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<rpp_msgs::msg::ErrorState>()
{
  return "rpp_msgs::msg::ErrorState";
}

template<>
inline const char * name<rpp_msgs::msg::ErrorState>()
{
  return "rpp_msgs/msg/ErrorState";
}

template<>
struct has_fixed_size<rpp_msgs::msg::ErrorState>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<rpp_msgs::msg::ErrorState>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<rpp_msgs::msg::ErrorState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RPP_MSGS__MSG__DETAIL__ERROR_STATE__TRAITS_HPP_
