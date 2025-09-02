// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rpp_msgs:msg/SingleMotorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SINGLE_MOTOR_STATE__STRUCT_H_
#define RPP_MSGS__MSG__DETAIL__SINGLE_MOTOR_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/SingleMotorState in the package rpp_msgs.
typedef struct rpp_msgs__msg__SingleMotorState
{
  float theta;
  float omega;
  /// no use
  float voltage;
  /// no use
  float current;
  /// no use
  float temperature;
  /// no use
  float rpm;
  /// no use
  int32_t tick;
  /// no use
  uint32_t fault;
} rpp_msgs__msg__SingleMotorState;

// Struct for a sequence of rpp_msgs__msg__SingleMotorState.
typedef struct rpp_msgs__msg__SingleMotorState__Sequence
{
  rpp_msgs__msg__SingleMotorState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rpp_msgs__msg__SingleMotorState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__SINGLE_MOTOR_STATE__STRUCT_H_
