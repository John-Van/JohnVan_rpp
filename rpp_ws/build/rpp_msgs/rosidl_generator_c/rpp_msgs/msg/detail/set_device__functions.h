// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from rpp_msgs:msg/SetDevice.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_DEVICE__FUNCTIONS_H_
#define RPP_MSGS__MSG__DETAIL__SET_DEVICE__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "rpp_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "rpp_msgs/msg/detail/set_device__struct.h"

/// Initialize msg/SetDevice message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * rpp_msgs__msg__SetDevice
 * )) before or use
 * rpp_msgs__msg__SetDevice__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetDevice__init(rpp_msgs__msg__SetDevice * msg);

/// Finalize msg/SetDevice message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
void
rpp_msgs__msg__SetDevice__fini(rpp_msgs__msg__SetDevice * msg);

/// Create msg/SetDevice message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * rpp_msgs__msg__SetDevice__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
rpp_msgs__msg__SetDevice *
rpp_msgs__msg__SetDevice__create();

/// Destroy msg/SetDevice message.
/**
 * It calls
 * rpp_msgs__msg__SetDevice__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
void
rpp_msgs__msg__SetDevice__destroy(rpp_msgs__msg__SetDevice * msg);

/// Check for msg/SetDevice message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetDevice__are_equal(const rpp_msgs__msg__SetDevice * lhs, const rpp_msgs__msg__SetDevice * rhs);

/// Copy a msg/SetDevice message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetDevice__copy(
  const rpp_msgs__msg__SetDevice * input,
  rpp_msgs__msg__SetDevice * output);

/// Initialize array of msg/SetDevice messages.
/**
 * It allocates the memory for the number of elements and calls
 * rpp_msgs__msg__SetDevice__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetDevice__Sequence__init(rpp_msgs__msg__SetDevice__Sequence * array, size_t size);

/// Finalize array of msg/SetDevice messages.
/**
 * It calls
 * rpp_msgs__msg__SetDevice__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
void
rpp_msgs__msg__SetDevice__Sequence__fini(rpp_msgs__msg__SetDevice__Sequence * array);

/// Create array of msg/SetDevice messages.
/**
 * It allocates the memory for the array and calls
 * rpp_msgs__msg__SetDevice__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
rpp_msgs__msg__SetDevice__Sequence *
rpp_msgs__msg__SetDevice__Sequence__create(size_t size);

/// Destroy array of msg/SetDevice messages.
/**
 * It calls
 * rpp_msgs__msg__SetDevice__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
void
rpp_msgs__msg__SetDevice__Sequence__destroy(rpp_msgs__msg__SetDevice__Sequence * array);

/// Check for msg/SetDevice message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetDevice__Sequence__are_equal(const rpp_msgs__msg__SetDevice__Sequence * lhs, const rpp_msgs__msg__SetDevice__Sequence * rhs);

/// Copy an array of msg/SetDevice messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetDevice__Sequence__copy(
  const rpp_msgs__msg__SetDevice__Sequence * input,
  rpp_msgs__msg__SetDevice__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__SET_DEVICE__FUNCTIONS_H_
