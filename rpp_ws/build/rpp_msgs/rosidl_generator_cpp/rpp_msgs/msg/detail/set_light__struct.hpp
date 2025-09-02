// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rpp_msgs:msg/SetLight.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SET_LIGHT__STRUCT_HPP_
#define RPP_MSGS__MSG__DETAIL__SET_LIGHT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rpp_msgs__msg__SetLight __attribute__((deprecated))
#else
# define DEPRECATED__rpp_msgs__msg__SetLight __declspec(deprecated)
#endif

namespace rpp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SetLight_
{
  using Type = SetLight_<ContainerAllocator>;

  explicit SetLight_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->front_light = 0;
      this->rear_light = 0;
    }
  }

  explicit SetLight_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->front_light = 0;
      this->rear_light = 0;
    }
  }

  // field types and members
  using _front_light_type =
    uint8_t;
  _front_light_type front_light;
  using _rear_light_type =
    uint8_t;
  _rear_light_type rear_light;

  // setters for named parameter idiom
  Type & set__front_light(
    const uint8_t & _arg)
  {
    this->front_light = _arg;
    return *this;
  }
  Type & set__rear_light(
    const uint8_t & _arg)
  {
    this->rear_light = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rpp_msgs::msg::SetLight_<ContainerAllocator> *;
  using ConstRawPtr =
    const rpp_msgs::msg::SetLight_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rpp_msgs::msg::SetLight_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rpp_msgs::msg::SetLight_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::SetLight_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::SetLight_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::SetLight_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::SetLight_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rpp_msgs::msg::SetLight_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rpp_msgs::msg::SetLight_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rpp_msgs__msg__SetLight
    std::shared_ptr<rpp_msgs::msg::SetLight_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rpp_msgs__msg__SetLight
    std::shared_ptr<rpp_msgs::msg::SetLight_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetLight_ & other) const
  {
    if (this->front_light != other.front_light) {
      return false;
    }
    if (this->rear_light != other.rear_light) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetLight_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetLight_

// alias to use template instance with default allocator
using SetLight =
  rpp_msgs::msg::SetLight_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__SET_LIGHT__STRUCT_HPP_
