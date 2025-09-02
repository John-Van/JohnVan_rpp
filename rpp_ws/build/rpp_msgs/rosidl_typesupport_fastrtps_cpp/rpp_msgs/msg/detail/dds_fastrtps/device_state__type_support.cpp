// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from rpp_msgs:msg/DeviceState.idl
// generated code does not contain a copyright notice
#include "rpp_msgs/msg/detail/device_state__rosidl_typesupport_fastrtps_cpp.hpp"
#include "rpp_msgs/msg/detail/device_state__struct.hpp"

#include <limits>
#include <stdexcept>
#include <string>
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_fastrtps_cpp/wstring_conversion.hpp"
#include "fastcdr/Cdr.h"


// forward declaration of message dependencies and their conversion functions

namespace rpp_msgs
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_rpp_msgs
cdr_serialize(
  const rpp_msgs::msg::DeviceState & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: control_mode
  cdr << ros_message.control_mode;
  // Member: power_mode
  cdr << ros_message.power_mode;
  // Member: motor_mode
  cdr << ros_message.motor_mode;
  // Member: e_stop
  cdr << ros_message.e_stop;
  // Member: ts
  cdr << ros_message.ts;
  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_rpp_msgs
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  rpp_msgs::msg::DeviceState & ros_message)
{
  // Member: control_mode
  cdr >> ros_message.control_mode;

  // Member: power_mode
  cdr >> ros_message.power_mode;

  // Member: motor_mode
  cdr >> ros_message.motor_mode;

  // Member: e_stop
  cdr >> ros_message.e_stop;

  // Member: ts
  cdr >> ros_message.ts;

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_rpp_msgs
get_serialized_size(
  const rpp_msgs::msg::DeviceState & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: control_mode
  {
    size_t item_size = sizeof(ros_message.control_mode);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: power_mode
  {
    size_t item_size = sizeof(ros_message.power_mode);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: motor_mode
  {
    size_t item_size = sizeof(ros_message.motor_mode);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: e_stop
  {
    size_t item_size = sizeof(ros_message.e_stop);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: ts
  {
    size_t item_size = sizeof(ros_message.ts);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_rpp_msgs
max_serialized_size_DeviceState(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;


  // Member: control_mode
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: power_mode
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: motor_mode
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: e_stop
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint8_t);
    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: ts
  {
    size_t array_size = 1;

    last_member_size = array_size * sizeof(uint64_t);
    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = rpp_msgs::msg::DeviceState;
    is_plain =
      (
      offsetof(DataType, ts) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

static bool _DeviceState__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const rpp_msgs::msg::DeviceState *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _DeviceState__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<rpp_msgs::msg::DeviceState *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _DeviceState__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const rpp_msgs::msg::DeviceState *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _DeviceState__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_DeviceState(full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}

static message_type_support_callbacks_t _DeviceState__callbacks = {
  "rpp_msgs::msg",
  "DeviceState",
  _DeviceState__cdr_serialize,
  _DeviceState__cdr_deserialize,
  _DeviceState__get_serialized_size,
  _DeviceState__max_serialized_size
};

static rosidl_message_type_support_t _DeviceState__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_DeviceState__callbacks,
  get_message_typesupport_handle_function,
};

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace rpp_msgs

namespace rosidl_typesupport_fastrtps_cpp
{

template<>
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_EXPORT_rpp_msgs
const rosidl_message_type_support_t *
get_message_type_support_handle<rpp_msgs::msg::DeviceState>()
{
  return &rpp_msgs::msg::typesupport_fastrtps_cpp::_DeviceState__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, rpp_msgs, msg, DeviceState)() {
  return &rpp_msgs::msg::typesupport_fastrtps_cpp::_DeviceState__handle;
}

#ifdef __cplusplus
}
#endif
