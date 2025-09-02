// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rpp_msgs:msg/ErrorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__ERROR_STATE__STRUCT_HPP_
#define RPP_MSGS__MSG__DETAIL__ERROR_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rpp_msgs__msg__ErrorState __attribute__((deprecated))
#else
# define DEPRECATED__rpp_msgs__msg__ErrorState __declspec(deprecated)
#endif

namespace rpp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct ErrorState_
{
  using Type = ErrorState_<ContainerAllocator>;

  explicit ErrorState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->motor_error = 0;
      this->turn_motor_error = 0;
      this->bump_error = 0;
      this->communication_error = 0;
      this->ts = 0ll;
    }
  }

  explicit ErrorState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->motor_error = 0;
      this->turn_motor_error = 0;
      this->bump_error = 0;
      this->communication_error = 0;
      this->ts = 0ll;
    }
  }

  // field types and members
  using _motor_error_type =
    uint8_t;
  _motor_error_type motor_error;
  using _turn_motor_error_type =
    uint8_t;
  _turn_motor_error_type turn_motor_error;
  using _bump_error_type =
    uint8_t;
  _bump_error_type bump_error;
  using _communication_error_type =
    uint8_t;
  _communication_error_type communication_error;
  using _ts_type =
    int64_t;
  _ts_type ts;

  // setters for named parameter idiom
  Type & set__motor_error(
    const uint8_t & _arg)
  {
    this->motor_error = _arg;
    return *this;
  }
  Type & set__turn_motor_error(
    const uint8_t & _arg)
  {
    this->turn_motor_error = _arg;
    return *this;
  }
  Type & set__bump_error(
    const uint8_t & _arg)
  {
    this->bump_error = _arg;
    return *this;
  }
  Type & set__communication_error(
    const uint8_t & _arg)
  {
    this->communication_error = _arg;
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
    rpp_msgs::msg::ErrorState_<ContainerAllocator> *;
  using ConstRawPtr =
    const rpp_msgs::msg::ErrorState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rpp_msgs::msg::ErrorState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rpp_msgs::msg::ErrorState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::ErrorState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::ErrorState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::ErrorState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::ErrorState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rpp_msgs::msg::ErrorState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rpp_msgs::msg::ErrorState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rpp_msgs__msg__ErrorState
    std::shared_ptr<rpp_msgs::msg::ErrorState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rpp_msgs__msg__ErrorState
    std::shared_ptr<rpp_msgs::msg::ErrorState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const ErrorState_ & other) const
  {
    if (this->motor_error != other.motor_error) {
      return false;
    }
    if (this->turn_motor_error != other.turn_motor_error) {
      return false;
    }
    if (this->bump_error != other.bump_error) {
      return false;
    }
    if (this->communication_error != other.communication_error) {
      return false;
    }
    if (this->ts != other.ts) {
      return false;
    }
    return true;
  }
  bool operator!=(const ErrorState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct ErrorState_

// alias to use template instance with default allocator
using ErrorState =
  rpp_msgs::msg::ErrorState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__ERROR_STATE__STRUCT_HPP_
