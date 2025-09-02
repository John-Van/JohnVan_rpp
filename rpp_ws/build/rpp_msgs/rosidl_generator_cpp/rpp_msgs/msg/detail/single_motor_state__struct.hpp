// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from rpp_msgs:msg/SingleMotorState.idl
// generated code does not contain a copyright notice

#ifndef RPP_MSGS__MSG__DETAIL__SINGLE_MOTOR_STATE__STRUCT_HPP_
#define RPP_MSGS__MSG__DETAIL__SINGLE_MOTOR_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__rpp_msgs__msg__SingleMotorState __attribute__((deprecated))
#else
# define DEPRECATED__rpp_msgs__msg__SingleMotorState __declspec(deprecated)
#endif

namespace rpp_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct SingleMotorState_
{
  using Type = SingleMotorState_<ContainerAllocator>;

  explicit SingleMotorState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->theta = 0.0f;
      this->omega = 0.0f;
      this->voltage = 0.0f;
      this->current = 0.0f;
      this->temperature = 0.0f;
      this->rpm = 0.0f;
      this->tick = 0l;
      this->fault = 0ul;
    }
  }

  explicit SingleMotorState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->theta = 0.0f;
      this->omega = 0.0f;
      this->voltage = 0.0f;
      this->current = 0.0f;
      this->temperature = 0.0f;
      this->rpm = 0.0f;
      this->tick = 0l;
      this->fault = 0ul;
    }
  }

  // field types and members
  using _theta_type =
    float;
  _theta_type theta;
  using _omega_type =
    float;
  _omega_type omega;
  using _voltage_type =
    float;
  _voltage_type voltage;
  using _current_type =
    float;
  _current_type current;
  using _temperature_type =
    float;
  _temperature_type temperature;
  using _rpm_type =
    float;
  _rpm_type rpm;
  using _tick_type =
    int32_t;
  _tick_type tick;
  using _fault_type =
    uint32_t;
  _fault_type fault;

  // setters for named parameter idiom
  Type & set__theta(
    const float & _arg)
  {
    this->theta = _arg;
    return *this;
  }
  Type & set__omega(
    const float & _arg)
  {
    this->omega = _arg;
    return *this;
  }
  Type & set__voltage(
    const float & _arg)
  {
    this->voltage = _arg;
    return *this;
  }
  Type & set__current(
    const float & _arg)
  {
    this->current = _arg;
    return *this;
  }
  Type & set__temperature(
    const float & _arg)
  {
    this->temperature = _arg;
    return *this;
  }
  Type & set__rpm(
    const float & _arg)
  {
    this->rpm = _arg;
    return *this;
  }
  Type & set__tick(
    const int32_t & _arg)
  {
    this->tick = _arg;
    return *this;
  }
  Type & set__fault(
    const uint32_t & _arg)
  {
    this->fault = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    rpp_msgs::msg::SingleMotorState_<ContainerAllocator> *;
  using ConstRawPtr =
    const rpp_msgs::msg::SingleMotorState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<rpp_msgs::msg::SingleMotorState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<rpp_msgs::msg::SingleMotorState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::SingleMotorState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::SingleMotorState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      rpp_msgs::msg::SingleMotorState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<rpp_msgs::msg::SingleMotorState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<rpp_msgs::msg::SingleMotorState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<rpp_msgs::msg::SingleMotorState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__rpp_msgs__msg__SingleMotorState
    std::shared_ptr<rpp_msgs::msg::SingleMotorState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__rpp_msgs__msg__SingleMotorState
    std::shared_ptr<rpp_msgs::msg::SingleMotorState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SingleMotorState_ & other) const
  {
    if (this->theta != other.theta) {
      return false;
    }
    if (this->omega != other.omega) {
      return false;
    }
    if (this->voltage != other.voltage) {
      return false;
    }
    if (this->current != other.current) {
      return false;
    }
    if (this->temperature != other.temperature) {
      return false;
    }
    if (this->rpm != other.rpm) {
      return false;
    }
    if (this->tick != other.tick) {
      return false;
    }
    if (this->fault != other.fault) {
      return false;
    }
    return true;
  }
  bool operator!=(const SingleMotorState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SingleMotorState_

// alias to use template instance with default allocator
using SingleMotorState =
  rpp_msgs::msg::SingleMotorState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace rpp_msgs

#endif  // RPP_MSGS__MSG__DETAIL__SINGLE_MOTOR_STATE__STRUCT_HPP_
