#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "motor_pitch_sdk" for configuration ""
set_property(TARGET motor_pitch_sdk APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(motor_pitch_sdk PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libmotor_pitch_sdk.so"
  IMPORTED_SONAME_NOCONFIG "libmotor_pitch_sdk.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS motor_pitch_sdk )
list(APPEND _IMPORT_CHECK_FILES_FOR_motor_pitch_sdk "${_IMPORT_PREFIX}/lib/libmotor_pitch_sdk.so" )

# Import target "rpp_keyboard_teleoperate" for configuration ""
set_property(TARGET rpp_keyboard_teleoperate APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(rpp_keyboard_teleoperate PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/rpp_keyboard_teleoperate"
  )

list(APPEND _IMPORT_CHECK_TARGETS rpp_keyboard_teleoperate )
list(APPEND _IMPORT_CHECK_FILES_FOR_rpp_keyboard_teleoperate "${_IMPORT_PREFIX}/bin/rpp_keyboard_teleoperate" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
