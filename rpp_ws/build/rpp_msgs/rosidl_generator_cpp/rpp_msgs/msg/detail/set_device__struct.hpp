// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rpp_msgs:msg/SetDevice.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_DEVICE__STRUCT_HPP_
#define RPP_MSGS__MSG__DETAIL__SET_DEVICE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rpp_msgs__msg__SetDevice __attribute__((deprecated))
#else
# define DEPRECATED__rpp_msgs__msg__SetDevice __declspec(deprecated)
#endif

namespace rpp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SetDevice_
{
  using Type = SetDevice_<ContainerAllocator>;

  explicit SetDevice_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->control_mode = 0;
      this->power_mode = 0;
      this->motor_mode = 0;
    }
  }

  explicit SetDevice_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->control_mode = 0;
      this->power_mode = 0;
      this->motor_mode = 0;
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

  // constant declarations

  // pointer types
  using RawPtr =
    rpp_msgs::msg::SetDevice_<ContainerAllocator> *;
  using ConstRawPtr =
    const rpp_msgs::msg::SetDevice_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rpp_msgs::msg::SetDevice_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rpp_msgs::msg::SetDevice_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::SetDevice_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::SetDevice_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::SetDevice_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::SetDevice_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rpp_msgs::msg::SetDevice_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rpp_msgs::msg::SetDevice_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rpp_msgs__msg__SetDevice
    std::shared_ptr<rpp_msgs::msg::SetDevice_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rpp_msgs__msg__SetDevice
    std::shared_ptr<rpp_msgs::msg::SetDevice_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetDevice_ & other) const
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
    return true;
  }
  bool operator!=(const SetDevice_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetDevice_

// alias to use template instance with default allocator
using SetDevice =
  rpp_msgs::msg::SetDevice_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__SET_DEVICE__STRUCT_HPP_
