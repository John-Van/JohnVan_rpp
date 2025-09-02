// generated from rosidl_generator_py/resource/_idl_support.c.em
// with input from rpp_msgs:msg/DeviceState.idl
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
#include "rpp_msgs/msg/detail/device_state__struct.h"
#include "rpp_msgs/msg/detail/device_state__functions.h"


ROSIDL_GENERATOR_C_EXPORT
bool rpp_msgs__msg__device_state__convert_from_py(PyObject * _pymsg, void * _ros_message)
{
  // check that the passed message is of the expected Python class
  {
    char full_classname_dest[39];
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
    assert(strncmp("rpp_msgs.msg._device_state.DeviceState", full_classname_dest, 38) == 0);
  }
  rpp_msgs__msg__DeviceState * ros_message = _ros_message;
  {  // control_mode
    PyObject * field = PyObject_GetAttrString(_pymsg, "control_mode");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->control_mode = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // power_mode
    PyObject * field = PyObject_GetAttrString(_pymsg, "power_mode");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->power_mode = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // motor_mode
    PyObject * field = PyObject_GetAttrString(_pymsg, "motor_mode");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->motor_mode = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // e_stop
    PyObject * field = PyObject_GetAttrString(_pymsg, "e_stop");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->e_stop = (uint8_t)PyLong_AsUnsignedLong(field);
    Py_DECREF(field);
  }
  {  // ts
    PyObject * field = PyObject_GetAttrString(_pymsg, "ts");
    if (!field) {
      return false;
    }
    assert(PyLong_Check(field));
    ros_message->ts = PyLong_AsLongLong(field);
    Py_DECREF(field);
  }

  return true;
}

ROSIDL_GENERATOR_C_EXPORT
PyObject * rpp_msgs__msg__device_state__convert_to_py(void * raw_ros_message)
{
  /* NOTE(esteve): Call constructor of DeviceState */
  PyObject * _pymessage = NULL;
  {
    PyObject * pymessage_module = PyImport_ImportModule("rpp_msgs.msg._device_state");
    assert(pymessage_module);
    PyObject * pymessage_class = PyObject_GetAttrString(pymessage_module, "DeviceState");
    assert(pymessage_class);
    Py_DECREF(pymessage_module);
    _pymessage = PyObject_CallObject(pymessage_class, NULL);
    Py_DECREF(pymessage_class);
    if (!_pymessage) {
      return NULL;
    }
  }
  rpp_msgs__msg__DeviceState * ros_message = (rpp_msgs__msg__DeviceState *)raw_ros_message;
  {  // control_mode
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->control_mode);
    {
      int rc = PyObject_SetAttrString(_pymessage, "control_mode", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // power_mode
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->power_mode);
    {
      int rc = PyObject_SetAttrString(_pymessage, "power_mode", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // motor_mode
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->motor_mode);
    {
      int rc = PyObject_SetAttrString(_pymessage, "motor_mode", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // e_stop
    PyObject * field = NULL;
    field = PyLong_FromUnsignedLong(ros_message->e_stop);
    {
      int rc = PyObject_SetAttrString(_pymessage, "e_stop", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }
  {  // ts
    PyObject * field = NULL;
    field = PyLong_FromLongLong(ros_message->ts);
    {
      int rc = PyObject_SetAttrString(_pymessage, "ts", field);
      Py_DECREF(field);
      if (rc) {
        return NULL;
      }
    }
  }

  // ownership of _pymessage is transferred to the caller
  return _pymessage;
}
