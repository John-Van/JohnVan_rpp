// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rpp_msgs:msg/SetErrorMask.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_ERROR_MASK__STRUCT_H_
#define RPP_MSGS__MSG__DETAIL__SET_ERROR_MASK__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/SetErrorMask in the package rpp_msgs.
typedef struct rpp_msgs__msg__SetErrorMask
{
  uint8_t motor_mask;
  uint8_t bump_mask;
} rpp_msgs__msg__SetErrorMask;

// Struct for a sequence of rpp_msgs__msg__SetErrorMask.
typedef struct rpp_msgs__msg__SetErrorMask__Sequence
{
  rpp_msgs__msg__SetErrorMask * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rpp_msgs__msg__SetErrorMask__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__SET_ERROR_MASK__STRUCT_H_
