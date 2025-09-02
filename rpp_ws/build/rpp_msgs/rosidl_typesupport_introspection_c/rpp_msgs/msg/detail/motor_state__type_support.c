// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from rpp_msgs:msg/MotorState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "rpp_msgs/msg/detail/motor_state__rosidl_typesupport_introspection_c.h"
#include "rpp_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rpp_msgs/msg/detail/motor_state__functions.h"
#include "rpp_msgs/msg/detail/motor_state__struct.h"


// Include directives for member types
// Member `motor_states`
#include "rpp_msgs/msg/single_motor_state.h"
// Member `motor_states`
#include "rpp_msgs/msg/detail/single_motor_state__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  rpp_msgs__msg__MotorState__init(message_memory);
}

void rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_fini_function(void * message_memory)
{
  rpp_msgs__msg__MotorState__fini(message_memory);
}

size_t rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__size_function__MotorState__motor_states(
  const void * untyped_member)
{
  (void)untyped_member;
  return 4;
}

const void * rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__get_const_function__MotorState__motor_states(
  const void * untyped_member, size_t index)
{
  const rpp_msgs__msg__SingleMotorState * member =
    (const rpp_msgs__msg__SingleMotorState *)(untyped_member);
  return &member[index];
}

void * rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__get_function__MotorState__motor_states(
  void * untyped_member, size_t index)
{
  rpp_msgs__msg__SingleMotorState * member =
    (rpp_msgs__msg__SingleMotorState *)(untyped_member);
  return &member[index];
}

void rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__fetch_function__MotorState__motor_states(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const rpp_msgs__msg__SingleMotorState * item =
    ((const rpp_msgs__msg__SingleMotorState *)
    rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__get_const_function__MotorState__motor_states(untyped_member, index));
  rpp_msgs__msg__SingleMotorState * value =
    (rpp_msgs__msg__SingleMotorState *)(untyped_value);
  *value = *item;
}

void rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__assign_function__MotorState__motor_states(
  void * untyped_member, size_t index, const void * untyped_value)
{
  rpp_msgs__msg__SingleMotorState * item =
    ((rpp_msgs__msg__SingleMotorState *)
    rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__get_function__MotorState__motor_states(untyped_member, index));
  const rpp_msgs__msg__SingleMotorState * value =
    (const rpp_msgs__msg__SingleMotorState *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_message_member_array[2] = {
  {
    "motor_states",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    4,  // array size
    false,  // is upper bound
    offsetof(rpp_msgs__msg__MotorState, motor_states),  // bytes offset in struct
    NULL,  // default value
    rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__size_function__MotorState__motor_states,  // size() function pointer
    rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__get_const_function__MotorState__motor_states,  // get_const(index) function pointer
    rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__get_function__MotorState__motor_states,  // get(index) function pointer
    rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__fetch_function__MotorState__motor_states,  // fetch(index, &value) function pointer
    rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__assign_function__MotorState__motor_states,  // assign(index, value) function pointer
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
    offsetof(rpp_msgs__msg__MotorState, ts),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_message_members = {
  "rpp_msgs__msg",  // message namespace
  "MotorState",  // message name
  2,  // number of fields
  sizeof(rpp_msgs__msg__MotorState),
  rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_message_member_array,  // message members
  rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_init_function,  // function to initialize message memory (memory has to be allocated)
  rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_message_type_support_handle = {
  0,
  &rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_rpp_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rpp_msgs, msg, MotorState)() {
  rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, rpp_msgs, msg, SingleMotorState)();
  if (!rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_message_type_support_handle.typesupport_identifier) {
    rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &rpp_msgs__msg__MotorState__rosidl_typesupport_introspection_c__MotorState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
