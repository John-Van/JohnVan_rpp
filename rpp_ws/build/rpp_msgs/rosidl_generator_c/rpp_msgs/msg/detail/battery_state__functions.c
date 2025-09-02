// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rpp_msgs:msg/BatteryState.idl
// generated code does not contain a copyright notice
#include "rpp_msgs/msg/detail/battery_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
rpp_msgs__msg__BatteryState__init(rpp_msgs__msg__BatteryState * msg)
{
  if (!msg) {
    return false;
  }
  // percentage
  // temperature
  // voltage
  // current
  // ts
  return true;
}

void
rpp_msgs__msg__BatteryState__fini(rpp_msgs__msg__BatteryState * msg)
{
  if (!msg) {
    return;
  }
  // percentage
  // temperature
  // voltage
  // current
  // ts
}

bool
rpp_msgs__msg__BatteryState__are_equal(const rpp_msgs__msg__BatteryState * lhs, const rpp_msgs__msg__BatteryState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // percentage
  if (lhs->percentage != rhs->percentage) {
    return false;
  }
  // temperature
  if (lhs->temperature != rhs->temperature) {
    return false;
  }
  // voltage
  if (lhs->voltage != rhs->voltage) {
    return false;
  }
  // current
  if (lhs->current != rhs->current) {
    return false;
  }
  // ts
  if (lhs->ts != rhs->ts) {
    return false;
  }
  return true;
}

bool
rpp_msgs__msg__BatteryState__copy(
  const rpp_msgs__msg__BatteryState * input,
  rpp_msgs__msg__BatteryState * output)
{
  if (!input || !output) {
    return false;
  }
  // percentage
  output->percentage = input->percentage;
  // temperature
  output->temperature = input->temperature;
  // voltage
  output->voltage = input->voltage;
  // current
  output->current = input->current;
  // ts
  output->ts = input->ts;
  return true;
}

rpp_msgs__msg__BatteryState *
rpp_msgs__msg__BatteryState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rpp_msgs__msg__BatteryState * msg = (rpp_msgs__msg__BatteryState *)allocator.allocate(sizeof(rpp_msgs__msg__BatteryState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rpp_msgs__msg__BatteryState));
  bool success = rpp_msgs__msg__BatteryState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rpp_msgs__msg__BatteryState__destroy(rpp_msgs__msg__BatteryState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rpp_msgs__msg__BatteryState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rpp_msgs__msg__BatteryState__Sequence__init(rpp_msgs__msg__BatteryState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rpp_msgs__msg__BatteryState * data = NULL;

  if (size) {
    data = (rpp_msgs__msg__BatteryState *)allocator.zero_allocate(size, sizeof(rpp_msgs__msg__BatteryState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rpp_msgs__msg__BatteryState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rpp_msgs__msg__BatteryState__fini(&data[i - 1]);
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
rpp_msgs__msg__BatteryState__Sequence__fini(rpp_msgs__msg__BatteryState__Sequence * array)
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
      rpp_msgs__msg__BatteryState__fini(&array->data[i]);
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

rpp_msgs__msg__BatteryState__Sequence *
rpp_msgs__msg__BatteryState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rpp_msgs__msg__BatteryState__Sequence * array = (rpp_msgs__msg__BatteryState__Sequence *)allocator.allocate(sizeof(rpp_msgs__msg__BatteryState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rpp_msgs__msg__BatteryState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rpp_msgs__msg__BatteryState__Sequence__destroy(rpp_msgs__msg__BatteryState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rpp_msgs__msg__BatteryState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rpp_msgs__msg__BatteryState__Sequence__are_equal(const rpp_msgs__msg__BatteryState__Sequence * lhs, const rpp_msgs__msg__BatteryState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rpp_msgs__msg__BatteryState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rpp_msgs__msg__BatteryState__Sequence__copy(
  const rpp_msgs__msg__BatteryState__Sequence * input,
  rpp_msgs__msg__BatteryState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rpp_msgs__msg__BatteryState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    rpp_msgs__msg__BatteryState * data =
      (rpp_msgs__msg__BatteryState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rpp_msgs__msg__BatteryState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          rpp_msgs__msg__BatteryState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!rpp_msgs__msg__BatteryState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
