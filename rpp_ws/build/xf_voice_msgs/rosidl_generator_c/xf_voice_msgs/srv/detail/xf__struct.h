// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from xf_voice_msgs:srv/Xf.idl
// generated code does not contain a copyright notice

#ifndef XF_VOICE_MSGS__SRV__DETAIL__XF__STRUCT_H_
#define XF_VOICE_MSGS__SRV__DETAIL__XF__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'text'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/Xf in the package xf_voice_msgs.
typedef struct xf_voice_msgs__srv__Xf_Request
{
  rosidl_runtime_c__String text;
} xf_voice_msgs__srv__Xf_Request;

// Struct for a sequence of xf_voice_msgs__srv__Xf_Request.
typedef struct xf_voice_msgs__srv__Xf_Request__Sequence
{
  xf_voice_msgs__srv__Xf_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xf_voice_msgs__srv__Xf_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in srv/Xf in the package xf_voice_msgs.
typedef struct xf_voice_msgs__srv__Xf_Response
{
  bool success;
  rosidl_runtime_c__String message;
} xf_voice_msgs__srv__Xf_Response;

// Struct for a sequence of xf_voice_msgs__srv__Xf_Response.
typedef struct xf_voice_msgs__srv__Xf_Response__Sequence
{
  xf_voice_msgs__srv__Xf_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} xf_voice_msgs__srv__Xf_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // XF_VOICE_MSGS__SRV__DETAIL__XF__STRUCT_H_
