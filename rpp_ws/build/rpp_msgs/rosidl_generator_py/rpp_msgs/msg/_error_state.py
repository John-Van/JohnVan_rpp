# generated from rosidl_generator_py/resource/_idl.py.em
# with input from rpp_msgs:msg/ErrorState.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ErrorState(type):
    """Metaclass of message 'ErrorState'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('rpp_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'rpp_msgs.msg.ErrorState')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__error_state
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__error_state
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__error_state
            cls._TYPE_SUPPORT = module.type_support_msg__msg__error_state
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__error_state

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ErrorState(metaclass=Metaclass_ErrorState):
    """Message class 'ErrorState'."""

    __slots__ = [
        '_motor_error',
        '_turn_motor_error',
        '_bump_error',
        '_communication_error',
        '_ts',
    ]

    _fields_and_field_types = {
        'motor_error': 'uint8',
        'turn_motor_error': 'uint8',
        'bump_error': 'uint8',
        'communication_error': 'uint8',
        'ts': 'int64',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('int64'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.motor_error = kwargs.get('motor_error', int())
        self.turn_motor_error = kwargs.get('turn_motor_error', int())
        self.bump_error = kwargs.get('bump_error', int())
        self.communication_error = kwargs.get('communication_error', int())
        self.ts = kwargs.get('ts', int())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.motor_error != other.motor_error:
            return False
        if self.turn_motor_error != other.turn_motor_error:
            return False
        if self.bump_error != other.bump_error:
            return False
        if self.communication_error != other.communication_error:
            return False
        if self.ts != other.ts:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def motor_error(self):
        """Message field 'motor_error'."""
        return self._motor_error

    @motor_error.setter
    def motor_error(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'motor_error' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'motor_error' field must be an unsigned integer in [0, 255]"
        self._motor_error = value

    @builtins.property
    def turn_motor_error(self):
        """Message field 'turn_motor_error'."""
        return self._turn_motor_error

    @turn_motor_error.setter
    def turn_motor_error(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'turn_motor_error' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'turn_motor_error' field must be an unsigned integer in [0, 255]"
        self._turn_motor_error = value

    @builtins.property
    def bump_error(self):
        """Message field 'bump_error'."""
        return self._bump_error

    @bump_error.setter
    def bump_error(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'bump_error' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'bump_error' field must be an unsigned integer in [0, 255]"
        self._bump_error = value

    @builtins.property
    def communication_error(self):
        """Message field 'communication_error'."""
        return self._communication_error

    @communication_error.setter
    def communication_error(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'communication_error' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'communication_error' field must be an unsigned integer in [0, 255]"
        self._communication_error = value

    @builtins.property
    def ts(self):
        """Message field 'ts'."""
        return self._ts

    @ts.setter
    def ts(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'ts' field must be of type 'int'"
            assert value >= -9223372036854775808 and value < 9223372036854775808, \
                "The 'ts' field must be an integer in [-9223372036854775808, 9223372036854775807]"
        self._ts = value
