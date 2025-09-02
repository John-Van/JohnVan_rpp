// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rpp_msgs:msg/DeviceState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__DEVICE_STATE__STRUCT_H_
#define RPP_MSGS__MSG__DETAIL__DEVICE_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/DeviceState in the package rpp_msgs.
typedef struct rpp_msgs__msg__DeviceState
{
  /// 0 - idle,1 - rs232, 2 - remote
  uint8_t control_mode;
  uint8_t power_mode;
  uint8_t motor_mode;
  uint8_t e_stop;
  /// time stamp in nanoseconds, since epoch
  int64_t ts;
} rpp_msgs__msg__DeviceState;

// Struct for a sequence of rpp_msgs__msg__DeviceState.
typedef struct rpp_msgs__msg__DeviceState__Sequence
{
  rpp_msgs__msg__DeviceState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rpp_msgs__msg__DeviceState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__DEVICE_STATE__STRUCT_H_
