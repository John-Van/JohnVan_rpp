// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from rpp_msgs:msg/SetLight.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_LIGHT__FUNCTIONS_H_
#define RPP_MSGS__MSG__DETAIL__SET_LIGHT__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "rpp_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "rpp_msgs/msg/detail/set_light__struct.h"

/// Initialize msg/SetLight message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * rpp_msgs__msg__SetLight
 * )) before or use
 * rpp_msgs__msg__SetLight__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetLight__init(rpp_msgs__msg__SetLight * msg);

/// Finalize msg/SetLight message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
void
rpp_msgs__msg__SetLight__fini(rpp_msgs__msg__SetLight * msg);

/// Create msg/SetLight message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * rpp_msgs__msg__SetLight__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
rpp_msgs__msg__SetLight *
rpp_msgs__msg__SetLight__create();

/// Destroy msg/SetLight message.
/**
 * It calls
 * rpp_msgs__msg__SetLight__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
void
rpp_msgs__msg__SetLight__destroy(rpp_msgs__msg__SetLight * msg);

/// Check for msg/SetLight message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetLight__are_equal(const rpp_msgs__msg__SetLight * lhs, const rpp_msgs__msg__SetLight * rhs);

/// Copy a msg/SetLight message.
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
rpp_msgs__msg__SetLight__copy(
  const rpp_msgs__msg__SetLight * input,
  rpp_msgs__msg__SetLight * output);

/// Initialize array of msg/SetLight messages.
/**
 * It allocates the memory for the number of elements and calls
 * rpp_msgs__msg__SetLight__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetLight__Sequence__init(rpp_msgs__msg__SetLight__Sequence * array, size_t size);

/// Finalize array of msg/SetLight messages.
/**
 * It calls
 * rpp_msgs__msg__SetLight__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
void
rpp_msgs__msg__SetLight__Sequence__fini(rpp_msgs__msg__SetLight__Sequence * array);

/// Create array of msg/SetLight messages.
/**
 * It allocates the memory for the array and calls
 * rpp_msgs__msg__SetLight__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
rpp_msgs__msg__SetLight__Sequence *
rpp_msgs__msg__SetLight__Sequence__create(size_t size);

/// Destroy array of msg/SetLight messages.
/**
 * It calls
 * rpp_msgs__msg__SetLight__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
void
rpp_msgs__msg__SetLight__Sequence__destroy(rpp_msgs__msg__SetLight__Sequence * array);

/// Check for msg/SetLight message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_rpp_msgs
bool
rpp_msgs__msg__SetLight__Sequence__are_equal(const rpp_msgs__msg__SetLight__Sequence * lhs, const rpp_msgs__msg__SetLight__Sequence * rhs);

/// Copy an array of msg/SetLight messages.
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
rpp_msgs__msg__SetLight__Sequence__copy(
  const rpp_msgs__msg__SetLight__Sequence * input,
  rpp_msgs__msg__SetLight__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // RPP_MSGS__MSG__DETAIL__SET_LIGHT__FUNCTIONS_H_
