// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rpp_msgs:msg/SetDevice.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_DEVICE__STRUCT_H_
#define RPP_MSGS__MSG__DETAIL__SET_DEVICE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/SetDevice in the package rpp_msgs.
typedef struct rpp_msgs__msg__SetDevice
{
  /// 0 - idle,1 - rs232, 2 - remote
  uint8_t control_mode;
  uint8_t power_mode;
  uint8_t motor_mode;
} rpp_msgs__msg__SetDevice;

// Struct for a sequence of rpp_msgs__msg__SetDevice.
typedef struct rpp_msgs__msg__SetDevice__Sequence
{
  rpp_msgs__msg__SetDevice * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rpp_msgs__msg__SetDevice__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__SET_DEVICE__STRUCT_H_
