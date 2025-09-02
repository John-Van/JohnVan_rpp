// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from rpp_msgs:msg/SingleMotorState.idl
// generated code does not contain a copyright notice
#include "rpp_msgs/msg/detail/single_motor_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
rpp_msgs__msg__SingleMotorState__init(rpp_msgs__msg__SingleMotorState * msg)
{
  if (!msg) {
    return false;
  }
  // theta
  // omega
  // voltage
  // current
  // temperature
  // rpm
  // tick
  // fault
  return true;
}

void
rpp_msgs__msg__SingleMotorState__fini(rpp_msgs__msg__SingleMotorState * msg)
{
  if (!msg) {
    return;
  }
  // theta
  // omega
  // voltage
  // current
  // temperature
  // rpm
  // tick
  // fault
}

bool
rpp_msgs__msg__SingleMotorState__are_equal(const rpp_msgs__msg__SingleMotorState * lhs, const rpp_msgs__msg__SingleMotorState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // theta
  if (lhs->theta != rhs->theta) {
    return false;
  }
  // omega
  if (lhs->omega != rhs->omega) {
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
  // temperature
  if (lhs->temperature != rhs->temperature) {
    return false;
  }
  // rpm
  if (lhs->rpm != rhs->rpm) {
    return false;
  }
  // tick
  if (lhs->tick != rhs->tick) {
    return false;
  }
  // fault
  if (lhs->fault != rhs->fault) {
    return false;
  }
  return true;
}

bool
rpp_msgs__msg__SingleMotorState__copy(
  const rpp_msgs__msg__SingleMotorState * input,
  rpp_msgs__msg__SingleMotorState * output)
{
  if (!input || !output) {
    return false;
  }
  // theta
  output->theta = input->theta;
  // omega
  output->omega = input->omega;
  // voltage
  output->voltage = input->voltage;
  // current
  output->current = input->current;
  // temperature
  output->temperature = input->temperature;
  // rpm
  output->rpm = input->rpm;
  // tick
  output->tick = input->tick;
  // fault
  output->fault = input->fault;
  return true;
}

rpp_msgs__msg__SingleMotorState *
rpp_msgs__msg__SingleMotorState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rpp_msgs__msg__SingleMotorState * msg = (rpp_msgs__msg__SingleMotorState *)allocator.allocate(sizeof(rpp_msgs__msg__SingleMotorState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(rpp_msgs__msg__SingleMotorState));
  bool success = rpp_msgs__msg__SingleMotorState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
rpp_msgs__msg__SingleMotorState__destroy(rpp_msgs__msg__SingleMotorState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    rpp_msgs__msg__SingleMotorState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
rpp_msgs__msg__SingleMotorState__Sequence__init(rpp_msgs__msg__SingleMotorState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rpp_msgs__msg__SingleMotorState * data = NULL;

  if (size) {
    data = (rpp_msgs__msg__SingleMotorState *)allocator.zero_allocate(size, sizeof(rpp_msgs__msg__SingleMotorState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = rpp_msgs__msg__SingleMotorState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        rpp_msgs__msg__SingleMotorState__fini(&data[i - 1]);
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
rpp_msgs__msg__SingleMotorState__Sequence__fini(rpp_msgs__msg__SingleMotorState__Sequence * array)
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
      rpp_msgs__msg__SingleMotorState__fini(&array->data[i]);
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

rpp_msgs__msg__SingleMotorState__Sequence *
rpp_msgs__msg__SingleMotorState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rpp_msgs__msg__SingleMotorState__Sequence * array = (rpp_msgs__msg__SingleMotorState__Sequence *)allocator.allocate(sizeof(rpp_msgs__msg__SingleMotorState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = rpp_msgs__msg__SingleMotorState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
rpp_msgs__msg__SingleMotorState__Sequence__destroy(rpp_msgs__msg__SingleMotorState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    rpp_msgs__msg__SingleMotorState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
rpp_msgs__msg__SingleMotorState__Sequence__are_equal(const rpp_msgs__msg__SingleMotorState__Sequence * lhs, const rpp_msgs__msg__SingleMotorState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!rpp_msgs__msg__SingleMotorState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
rpp_msgs__msg__SingleMotorState__Sequence__copy(
  const rpp_msgs__msg__SingleMotorState__Sequence * input,
  rpp_msgs__msg__SingleMotorState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(rpp_msgs__msg__SingleMotorState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    rpp_msgs__msg__SingleMotorState * data =
      (rpp_msgs__msg__SingleMotorState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!rpp_msgs__msg__SingleMotorState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          rpp_msgs__msg__SingleMotorState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!rpp_msgs__msg__SingleMotorState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
