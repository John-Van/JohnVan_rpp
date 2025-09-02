// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from rpp_msgs:msg/SetErrorMask.idl
// generated code does not contain a copyright notice
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <stdbool.h>
#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include "numpy/ndarrayobject.h"
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif
#include "rosidl_runtime_c/visibility_control.h"
#include "rpp_msgs/msg/detail/set_error_mask__struct.h"
#include "rpp_msgs/msg/detail/set_error_mask__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool rpp_msgs__msg__set_error_mask__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[42];
    {
      char * class_name = NULL;
      char * module_name = NULL;
      {
        PyObject * class_attr = PyObject_GetAttrString(_pymsg, "__class__");
        if (class_attr) {
          PyObject * name_attr = PyObject_GetAttrString(class_attr, "__name__");
          if (name_attr) {
            class_name = (char *)PyUnicode_1BYTE_DATA(name_attr);
            Py_DECREF(name_attr);
          }
          PyObject * module_attr = PyObject_GetAttrString(class_attr, "__module__");
          if (module_attr) {
            module_name = (char *)PyUnicode_1BYTE_DATA(module_attr);
            Py_DECREF(module_attr);
          }
          Py_DECREF(class_attr);
        }
      }
      if (!class_name || !module_name) {
        return false;
      }
      snprintf(full_classname_dest, sizeof(full_classname_dest), "%s.%s", module_name, class_name);
    }
    assert(strncmp("rpp_msgs.msg._set_error_mask.SetErrorMask", full_classname_dest, 41) == 0);
  }
  rpp_msgs__msg__SetErrorMask * ros_message = _ros_message;
  {  // motor_mask
    PyObject * field = PyObject_GetAttrString(_pymsg, "motor_mask");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->motor_mask = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // bump_mask
    PyObject * field = PyObject_GetAttrString(_pymsg, "bump_mask");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->bump_mask = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * rpp_msgs__msg__set_error_mask__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of SetErrorMask */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("rpp_msgs.msg._set_error_mask");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "SetErrorMask");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  rpp_msgs__msg__SetErrorMask * ros_message = (rpp_msgs__msg__SetErrorMask *)raw_ros_message;
  {  // motor_mask
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->motor_mask);
    {
      int rc = PyObject_SetAttrString(_pymessage, "motor_mask", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // bump_mask
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->bump_mask);
    {
      int rc = PyObject_SetAttrString(_pymessage, "bump_mask", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
