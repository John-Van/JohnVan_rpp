#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ira_laser_tools::ira_laser_tools" for configuration ""
set_property(TARGET ira_laser_tools::ira_laser_tools APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ira_laser_tools::ira_laser_tools PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libira_laser_tools.so"
  IMPORTED_SONAME_NOCONFIG "libira_laser_tools.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS ira_laser_tools::ira_laser_tools )
list(APPEND _IMPORT_CHECK_FILES_FOR_ira_laser_tools::ira_laser_tools "${_IMPORT_PREFIX}/lib/libira_laser_tools.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
