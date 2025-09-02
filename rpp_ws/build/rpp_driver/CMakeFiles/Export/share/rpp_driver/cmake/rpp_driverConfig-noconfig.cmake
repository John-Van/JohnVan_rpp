#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "rpp_driver" for configuration ""
set_property(TARGET rpp_driver APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(rpp_driver PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/librpp_driver.so"
  IMPORTED_SONAME_NOCONFIG "librpp_driver.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS rpp_driver )
list(APPEND _IMPORT_CHECK_FILES_FOR_rpp_driver "${_IMPORT_PREFIX}/lib/librpp_driver.so" )

# Import target "rpp_keyboard_teleoperate" for configuration ""
set_property(TARGET rpp_keyboard_teleoperate APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(rpp_keyboard_teleoperate PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/bin/rpp_keyboard_teleoperate"
  )

list(APPEND _IMPORT_CHECK_TARGETS rpp_keyboard_teleoperate )
list(APPEND _IMPORT_CHECK_FILES_FOR_rpp_keyboard_teleoperate "${_IMPORT_PREFIX}/bin/rpp_keyboard_teleoperate" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
