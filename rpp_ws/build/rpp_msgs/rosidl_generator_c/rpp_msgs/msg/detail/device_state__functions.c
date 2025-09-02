// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rpp_msgs:msg/DeviceState.idl
// generated code does not contain a copyright notice
#include "rpp_msgs/msg/detail/device_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
rpp_msgs__msg__DeviceState__init(rpp_msgs__msg__DeviceState * msg)
{
  if (!msg) {
    return false;
  }
  // control_mode
  // power_mode
  // motor_mode
  // e_stop
  // ts
  return true;
}

void
rpp_msgs__msg__DeviceState__fini(rpp_msgs__msg__DeviceState * msg)
{
  if (!msg) {
    return;
  }
  // control_mode
  // power_mode
  // motor_mode
  // e_stop
  // ts
}

bool
rpp_msgs__msg__DeviceState__are_equal(const rpp_msgs__msg__DeviceState * lhs, const rpp_msgs__msg__DeviceState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // control_mode
  if (lhs->control_mode != rhs->control_mode) {
    return false;
  }
  // power_mode
  if (lhs->power_mode != rhs->power_mode) {
    return false;
  }
  // motor_mode
  if (lhs->motor_mode != rhs->motor_mode) {
    return false;
  }
  // e_stop
  if (lhs->e_stop != rhs->e_stop) {
    return false;
  }
  // ts
  if (lhs->ts != rhs->ts) {
    return false;
  }
  return true;
}

bool
rpp_msgs__msg__DeviceState__copy(
  const rpp_msgs__msg__DeviceState * input,
  rpp_msgs__msg__DeviceState * output)
{
  if (!input || !output) {
    return false;
  }
  // control_mode
  output->control_mode = input->control_mode;
  // power_mode
  output->power_mode = input->power_mode;
  // motor_mode
  output->motor_mode = input->motor_mode;
  // e_stop
  output->e_stop = input->e_stop;
  // ts
  output->ts = input->ts;
  return true;
}

rpp_msgs__msg__DeviceState *
rpp_msgs__msg__DeviceState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rpp_msgs__msg__DeviceState * msg = (rpp_msgs__msg__DeviceState *)allocator.allocate(sizeof(rpp_msgs__msg__DeviceState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rpp_msgs__msg__DeviceState));
  bool success = rpp_msgs__msg__DeviceState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rpp_msgs__msg__DeviceState__destroy(rpp_msgs__msg__DeviceState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rpp_msgs__msg__DeviceState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rpp_msgs__msg__DeviceState__Sequence__init(rpp_msgs__msg__DeviceState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rpp_msgs__msg__DeviceState * data = NULL;

  if (size) {
    data = (rpp_msgs__msg__DeviceState *)allocator.zero_allocate(size, sizeof(rpp_msgs__msg__DeviceState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rpp_msgs__msg__DeviceState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rpp_msgs__msg__DeviceState__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
rpp_msgs__msg__DeviceState__Sequence__fini(rpp_msgs__msg__DeviceState__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      rpp_msgs__msg__DeviceState__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

rpp_msgs__msg__DeviceState__Sequence *
rpp_msgs__msg__DeviceState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rpp_msgs__msg__DeviceState__Sequence * array = (rpp_msgs__msg__DeviceState__Sequence *)allocator.allocate(sizeof(rpp_msgs__msg__DeviceState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rpp_msgs__msg__DeviceState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rpp_msgs__msg__DeviceState__Sequence__destroy(rpp_msgs__msg__DeviceState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rpp_msgs__msg__DeviceState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rpp_msgs__msg__DeviceState__Sequence__are_equal(const rpp_msgs__msg__DeviceState__Sequence * lhs, const rpp_msgs__msg__DeviceState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rpp_msgs__msg__DeviceState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rpp_msgs__msg__DeviceState__Sequence__copy(
  const rpp_msgs__msg__DeviceState__Sequence * input,
  rpp_msgs__msg__DeviceState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rpp_msgs__msg__DeviceState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    rpp_msgs__msg__DeviceState * data =
      (rpp_msgs__msg__DeviceState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rpp_msgs__msg__DeviceState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          rpp_msgs__msg__DeviceState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!rpp_msgs__msg__DeviceState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
