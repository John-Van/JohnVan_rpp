// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rpp_msgs:msg/MotorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__MOTOR_STATE__STRUCT_H_
#define RPP_MSGS__MSG__DETAIL__MOTOR_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'motor_states'
#include "rpp_msgs/msg/detail/single_motor_state__struct.h"

/// Struct defined in msg/MotorState in the package rpp_msgs.
typedef struct rpp_msgs__msg__MotorState
{
  rpp_msgs__msg__SingleMotorState motor_states[4];
  /// time stamp in nanoseconds, since epoch
  int64_t ts;
} rpp_msgs__msg__MotorState;

// Struct for a sequence of rpp_msgs__msg__MotorState.
typedef struct rpp_msgs__msg__MotorState__Sequence
{
  rpp_msgs__msg__MotorState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rpp_msgs__msg__MotorState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__MOTOR_STATE__STRUCT_H_
