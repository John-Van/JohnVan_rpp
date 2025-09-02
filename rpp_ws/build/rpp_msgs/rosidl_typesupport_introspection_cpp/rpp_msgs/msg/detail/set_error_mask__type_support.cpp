// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from rpp_msgs:msg/SetErrorMask.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "rpp_msgs/msg/detail/set_error_mask__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace rpp_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void SetErrorMask_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) rpp_msgs::msg::SetErrorMask(_init);
}

void SetErrorMask_fini_function(void * message_memory)
{
  auto typed_message = static_cast<rpp_msgs::msg::SetErrorMask *>(message_memory);
  typed_message->~SetErrorMask();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember SetErrorMask_message_member_array[2] = {
  {
    "motor_mask",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rpp_msgs::msg::SetErrorMask, motor_mask),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "bump_mask",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rpp_msgs::msg::SetErrorMask, bump_mask),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers SetErrorMask_message_members = {
  "rpp_msgs::msg",  // message namespace
  "SetErrorMask",  // message name
  2,  // number of fields
  sizeof(rpp_msgs::msg::SetErrorMask),
  SetErrorMask_message_member_array,  // message members
  SetErrorMask_init_function,  // function to initialize message memory (memory has to be allocated)
  SetErrorMask_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t SetErrorMask_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &SetErrorMask_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace rpp_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<rpp_msgs::msg::SetErrorMask>()
{
  return &::rpp_msgs::msg::rosidl_typesupport_introspection_cpp::SetErrorMask_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, rpp_msgs, msg, SetErrorMask)() {
  return &::rpp_msgs::msg::rosidl_typesupport_introspection_cpp::SetErrorMask_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
