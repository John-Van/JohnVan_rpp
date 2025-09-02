// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rpp_msgs:msg/ErrorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__ERROR_STATE__STRUCT_H_
#define RPP_MSGS__MSG__DETAIL__ERROR_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/ErrorState in the package rpp_msgs.
typedef struct rpp_msgs__msg__ErrorState
{
  uint8_t motor_error;
  uint8_t turn_motor_error;
  uint8_t bump_error;
  uint8_t communication_error;
  /// time stamp in nanoseconds, since epoch
  int64_t ts;
} rpp_msgs__msg__ErrorState;

// Struct for a sequence of rpp_msgs__msg__ErrorState.
typedef struct rpp_msgs__msg__ErrorState__Sequence
{
  rpp_msgs__msg__ErrorState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rpp_msgs__msg__ErrorState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__ERROR_STATE__STRUCT_H_
