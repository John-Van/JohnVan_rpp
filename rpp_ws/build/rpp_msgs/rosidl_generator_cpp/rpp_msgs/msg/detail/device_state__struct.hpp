// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rpp_msgs:msg/DeviceState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__DEVICE_STATE__STRUCT_HPP_
#define RPP_MSGS__MSG__DETAIL__DEVICE_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rpp_msgs__msg__DeviceState __attribute__((deprecated))
#else
# define DEPRECATED__rpp_msgs__msg__DeviceState __declspec(deprecated)
#endif

namespace rpp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct DeviceState_
{
  using Type = DeviceState_<ContainerAllocator>;

  explicit DeviceState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->control_mode = 0;
      this->power_mode = 0;
      this->motor_mode = 0;
      this->e_stop = 0;
      this->ts = 0ll;
    }
  }

  explicit DeviceState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->control_mode = 0;
      this->power_mode = 0;
      this->motor_mode = 0;
      this->e_stop = 0;
      this->ts = 0ll;
    }
  }

  // field types and members
  using _control_mode_type =
    uint8_t;
  _control_mode_type control_mode;
  using _power_mode_type =
    uint8_t;
  _power_mode_type power_mode;
  using _motor_mode_type =
    uint8_t;
  _motor_mode_type motor_mode;
  using _e_stop_type =
    uint8_t;
  _e_stop_type e_stop;
  using _ts_type =
    int64_t;
  _ts_type ts;

  // setters for named parameter idiom
  Type & set__control_mode(
    const uint8_t & _arg)
  {
    this->control_mode = _arg;
    return *this;
  }
  Type & set__power_mode(
    const uint8_t & _arg)
  {
    this->power_mode = _arg;
    return *this;
  }
  Type & set__motor_mode(
    const uint8_t & _arg)
  {
    this->motor_mode = _arg;
    return *this;
  }
  Type & set__e_stop(
    const uint8_t & _arg)
  {
    this->e_stop = _arg;
    return *this;
  }
  Type & set__ts(
    const int64_t & _arg)
  {
    this->ts = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rpp_msgs::msg::DeviceState_<ContainerAllocator> *;
  using ConstRawPtr =
    const rpp_msgs::msg::DeviceState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rpp_msgs::msg::DeviceState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rpp_msgs::msg::DeviceState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::DeviceState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::DeviceState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::DeviceState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::DeviceState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rpp_msgs::msg::DeviceState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rpp_msgs::msg::DeviceState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rpp_msgs__msg__DeviceState
    std::shared_ptr<rpp_msgs::msg::DeviceState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rpp_msgs__msg__DeviceState
    std::shared_ptr<rpp_msgs::msg::DeviceState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const DeviceState_ & other) const
  {
    if (this->control_mode != other.control_mode) {
      return false;
    }
    if (this->power_mode != other.power_mode) {
      return false;
    }
    if (this->motor_mode != other.motor_mode) {
      return false;
    }
    if (this->e_stop != other.e_stop) {
      return false;
    }
    if (this->ts != other.ts) {
      return false;
    }
    return true;
  }
  bool operator!=(const DeviceState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct DeviceState_

// alias to use template instance with default allocator
using DeviceState =
  rpp_msgs::msg::DeviceState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__DEVICE_STATE__STRUCT_HPP_
