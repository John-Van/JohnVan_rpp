// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rpp_msgs:msg/SetLight.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_LIGHT__STRUCT_H_
#define RPP_MSGS__MSG__DETAIL__SET_LIGHT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/SetLight in the package rpp_msgs.
typedef struct rpp_msgs__msg__SetLight
{
  uint8_t front_light;
  uint8_t rear_light;
} rpp_msgs__msg__SetLight;

// Struct for a sequence of rpp_msgs__msg__SetLight.
typedef struct rpp_msgs__msg__SetLight__Sequence
{
  rpp_msgs__msg__SetLight * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rpp_msgs__msg__SetLight__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__SET_LIGHT__STRUCT_H_
