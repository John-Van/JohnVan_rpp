// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rpp_msgs:msg/SetErrorMask.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_ERROR_MASK__STRUCT_HPP_
#define RPP_MSGS__MSG__DETAIL__SET_ERROR_MASK__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rpp_msgs__msg__SetErrorMask __attribute__((deprecated))
#else
# define DEPRECATED__rpp_msgs__msg__SetErrorMask __declspec(deprecated)
#endif

namespace rpp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SetErrorMask_
{
  using Type = SetErrorMask_<ContainerAllocator>;

  explicit SetErrorMask_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->motor_mask = 0;
      this->bump_mask = 0;
    }
  }

  explicit SetErrorMask_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->motor_mask = 0;
      this->bump_mask = 0;
    }
  }

  // field types and members
  using _motor_mask_type =
    uint8_t;
  _motor_mask_type motor_mask;
  using _bump_mask_type =
    uint8_t;
  _bump_mask_type bump_mask;

  // setters for named parameter idiom
  Type & set__motor_mask(
    const uint8_t & _arg)
  {
    this->motor_mask = _arg;
    return *this;
  }
  Type & set__bump_mask(
    const uint8_t & _arg)
  {
    this->bump_mask = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rpp_msgs::msg::SetErrorMask_<ContainerAllocator> *;
  using ConstRawPtr =
    const rpp_msgs::msg::SetErrorMask_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rpp_msgs::msg::SetErrorMask_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rpp_msgs::msg::SetErrorMask_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::SetErrorMask_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::SetErrorMask_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::SetErrorMask_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::SetErrorMask_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rpp_msgs::msg::SetErrorMask_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rpp_msgs::msg::SetErrorMask_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rpp_msgs__msg__SetErrorMask
    std::shared_ptr<rpp_msgs::msg::SetErrorMask_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rpp_msgs__msg__SetErrorMask
    std::shared_ptr<rpp_msgs::msg::SetErrorMask_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetErrorMask_ & other) const
  {
    if (this->motor_mask != other.motor_mask) {
      return false;
    }
    if (this->bump_mask != other.bump_mask) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetErrorMask_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetErrorMask_

// alias to use template instance with default allocator
using SetErrorMask =
  rpp_msgs::msg::SetErrorMask_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__SET_ERROR_MASK__STRUCT_HPP_
