// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rpp_msgs:msg/LightState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__LIGHT_STATE__STRUCT_HPP_
#define RPP_MSGS__MSG__DETAIL__LIGHT_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rpp_msgs__msg__LightState __attribute__((deprecated))
#else
# define DEPRECATED__rpp_msgs__msg__LightState __declspec(deprecated)
#endif

namespace rpp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct LightState_
{
  using Type = LightState_<ContainerAllocator>;

  explicit LightState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->front_light = 0;
      this->rear_light = 0;
      this->ts = 0ll;
    }
  }

  explicit LightState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->front_light = 0;
      this->rear_light = 0;
      this->ts = 0ll;
    }
  }

  // field types and members
  using _front_light_type =
    uint8_t;
  _front_light_type front_light;
  using _rear_light_type =
    uint8_t;
  _rear_light_type rear_light;
  using _ts_type =
    int64_t;
  _ts_type ts;

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
  Type & set__ts(
    const int64_t & _arg)
  {
    this->ts = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rpp_msgs::msg::LightState_<ContainerAllocator> *;
  using ConstRawPtr =
    const rpp_msgs::msg::LightState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rpp_msgs::msg::LightState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rpp_msgs::msg::LightState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::LightState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::LightState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::LightState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::LightState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rpp_msgs::msg::LightState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rpp_msgs::msg::LightState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rpp_msgs__msg__LightState
    std::shared_ptr<rpp_msgs::msg::LightState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rpp_msgs__msg__LightState
    std::shared_ptr<rpp_msgs::msg::LightState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const LightState_ & other) const
  {
    if (this->front_light != other.front_light) {
      return false;
    }
    if (this->rear_light != other.rear_light) {
      return false;
    }
    if (this->ts != other.ts) {
      return false;
    }
    return true;
  }
  bool operator!=(const LightState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct LightState_

// alias to use template instance with default allocator
using LightState =
  rpp_msgs::msg::LightState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__LIGHT_STATE__STRUCT_HPP_
