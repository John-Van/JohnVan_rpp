// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from xf_voice_msgs:srv/Xf.idl
// generated code does not contain a copyright notice

#ifndef XF_VOICE_MSGS__SRV__DETAIL__XF__STRUCT_HPP_
#define XF_VOICE_MSGS__SRV__DETAIL__XF__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__xf_voice_msgs__srv__Xf_Request __attribute__((deprecated))
#else
# define DEPRECATED__xf_voice_msgs__srv__Xf_Request __declspec(deprecated)
#endif

namespace xf_voice_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Xf_Request_
{
  using Type = Xf_Request_<ContainerAllocator>;

  explicit Xf_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->text = "";
    }
  }

  explicit Xf_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : text(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->text = "";
    }
  }

  // field types and members
  using _text_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _text_type text;

  // setters for named parameter idiom
  Type & set__text(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->text = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xf_voice_msgs::srv::Xf_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const xf_voice_msgs::srv::Xf_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xf_voice_msgs::srv::Xf_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xf_voice_msgs::srv::Xf_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xf_voice_msgs::srv::Xf_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xf_voice_msgs::srv::Xf_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xf_voice_msgs::srv::Xf_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xf_voice_msgs::srv::Xf_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xf_voice_msgs::srv::Xf_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xf_voice_msgs::srv::Xf_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xf_voice_msgs__srv__Xf_Request
    std::shared_ptr<xf_voice_msgs::srv::Xf_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xf_voice_msgs__srv__Xf_Request
    std::shared_ptr<xf_voice_msgs::srv::Xf_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Xf_Request_ & other) const
  {
    if (this->text != other.text) {
      return false;
    }
    return true;
  }
  bool operator!=(const Xf_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Xf_Request_

// alias to use template instance with default allocator
using Xf_Request =
  xf_voice_msgs::srv::Xf_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xf_voice_msgs


#ifndef _WIN32
# define DEPRECATED__xf_voice_msgs__srv__Xf_Response __attribute__((deprecated))
#else
# define DEPRECATED__xf_voice_msgs__srv__Xf_Response __declspec(deprecated)
#endif

namespace xf_voice_msgs
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct Xf_Response_
{
  using Type = Xf_Response_<ContainerAllocator>;

  explicit Xf_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  explicit Xf_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    xf_voice_msgs::srv::Xf_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const xf_voice_msgs::srv::Xf_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<xf_voice_msgs::srv::Xf_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<xf_voice_msgs::srv::Xf_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      xf_voice_msgs::srv::Xf_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<xf_voice_msgs::srv::Xf_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      xf_voice_msgs::srv::Xf_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<xf_voice_msgs::srv::Xf_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<xf_voice_msgs::srv::Xf_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<xf_voice_msgs::srv::Xf_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__xf_voice_msgs__srv__Xf_Response
    std::shared_ptr<xf_voice_msgs::srv::Xf_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__xf_voice_msgs__srv__Xf_Response
    std::shared_ptr<xf_voice_msgs::srv::Xf_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Xf_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const Xf_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Xf_Response_

// alias to use template instance with default allocator
using Xf_Response =
  xf_voice_msgs::srv::Xf_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace xf_voice_msgs

namespace xf_voice_msgs
{

namespace srv
{

struct Xf
{
  using Request = xf_voice_msgs::srv::Xf_Request;
  using Response = xf_voice_msgs::srv::Xf_Response;
};

}  // namespace srv

}  // namespace xf_voice_msgs

#endif  // XF_VOICE_MSGS__SRV__DETAIL__XF__STRUCT_HPP_
