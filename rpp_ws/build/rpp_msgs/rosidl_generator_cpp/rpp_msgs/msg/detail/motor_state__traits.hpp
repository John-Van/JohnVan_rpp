// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from rpp_msgs:msg/MotorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__MOTOR_STATE__TRAITS_HPP_
#define RPP_MSGS__MSG__DETAIL__MOTOR_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "rpp_msgs/msg/detail/motor_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'motor_states'
#include "rpp_msgs/msg/detail/single_motor_state__traits.hpp"

namespace rpp_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const MotorState & msg,
  std::ostream & out)
{
  out << "{";
  // member: motor_states
  {
    if (msg.motor_states.size() == 0) {
      out << "motor_states: []";
    } else {
      out << "motor_states: [";
      size_t pending_items = msg.motor_states.size();
      for (auto item : msg.motor_states) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
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
  const MotorState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: motor_states
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.motor_states.size() == 0) {
      out << "motor_states: []\n";
    } else {
      out << "motor_states:\n";
      for (auto item : msg.motor_states) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
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

inline std::string to_yaml(const MotorState & msg, bool use_flow_style = false)
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
  const rpp_msgs::msg::MotorState & msg,
  std::ostream & out, size_t indentation = 0)
{
  rpp_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use rpp_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const rpp_msgs::msg::MotorState & msg)
{
  return rpp_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<rpp_msgs::msg::MotorState>()
{
  return "rpp_msgs::msg::MotorState";
}

template<>
inline const char * name<rpp_msgs::msg::MotorState>()
{
  return "rpp_msgs/msg/MotorState";
}

template<>
struct has_fixed_size<rpp_msgs::msg::MotorState>
  : std::integral_constant<bool, has_fixed_size<rpp_msgs::msg::SingleMotorState>::value> {};

template<>
struct has_bounded_size<rpp_msgs::msg::MotorState>
  : std::integral_constant<bool, has_bounded_size<rpp_msgs::msg::SingleMotorState>::value> {};

template<>
struct is_message<rpp_msgs::msg::MotorState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // RPP_MSGS__MSG__DETAIL__MOTOR_STATE__TRAITS_HPP_
