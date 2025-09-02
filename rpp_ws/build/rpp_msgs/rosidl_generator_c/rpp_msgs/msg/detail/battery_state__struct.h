// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from rpp_msgs:msg/BatteryState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__BATTERY_STATE__STRUCT_H_
#define RPP_MSGS__MSG__DETAIL__BATTERY_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/BatteryState in the package rpp_msgs.
typedef struct rpp_msgs__msg__BatteryState
{
  uint8_t percentage;
  float temperature;
  float voltage;
  float current;
  /// time stamp in nanoseconds, since epoch
  int64_t ts;
} rpp_msgs__msg__BatteryState;

// Struct for a sequence of rpp_msgs__msg__BatteryState.
typedef struct rpp_msgs__msg__BatteryState__Sequence
{
  rpp_msgs__msg__BatteryState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rpp_msgs__msg__BatteryState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__BATTERY_STATE__STRUCT_H_
