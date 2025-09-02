// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from rpp_msgs:msg/ErrorState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "rpp_msgs/msg/detail/error_state__rosidl_typesupport_introspection_c.h"
#include "rpp_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rpp_msgs/msg/detail/error_state__functions.h"
#include "rpp_msgs/msg/detail/error_state__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rpp_msgs__msg__ErrorState__init(message_memory);
}

void rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_fini_function(void * message_memory)
{
  rpp_msgs__msg__ErrorState__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_message_member_array[5] = {
  {
    "motor_error",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rpp_msgs__msg__ErrorState, motor_error),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "turn_motor_error",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rpp_msgs__msg__ErrorState, turn_motor_error),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "bump_error",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rpp_msgs__msg__ErrorState, bump_error),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "communication_error",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rpp_msgs__msg__ErrorState, communication_error),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "ts",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(rpp_msgs__msg__ErrorState, ts),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_message_members = {
  "rpp_msgs__msg",  // message namespace
  "ErrorState",  // message name
  5,  // number of fields
  sizeof(rpp_msgs__msg__ErrorState),
  rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_message_member_array,  // message members
  rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_init_function,  // function to initialize message memory (memory has to be allocated)
  rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_message_type_support_handle = {
  0,
  &rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rpp_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rpp_msgs, msg, ErrorState)() {
  if (!rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_message_type_support_handle.typesupport_identifier) {
    rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &rpp_msgs__msg__ErrorState__rosidl_typesupport_introspection_c__ErrorState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
