// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rpp_msgs:msg/LightState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__LIGHT_STATE__STRUCT_H_
#define RPP_MSGS__MSG__DETAIL__LIGHT_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/LightState in the package rpp_msgs.
typedef struct rpp_msgs__msg__LightState
{
  uint8_t front_light;
  uint8_t rear_light;
  /// time stamp in nanoseconds, since epoch
  int64_t ts;
} rpp_msgs__msg__LightState;

// Struct for a sequence of rpp_msgs__msg__LightState.
typedef struct rpp_msgs__msg__LightState__Sequence
{
  rpp_msgs__msg__LightState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rpp_msgs__msg__LightState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__LIGHT_STATE__STRUCT_H_
