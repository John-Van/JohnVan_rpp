# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_xf_voice_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED xf_voice_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(xf_voice_FOUND FALSE)
  elseif(NOT xf_voice_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(xf_voice_FOUND FALSE)
  endif()
  return()
endif()
set(_xf_voice_CONFIG_INCLUDED TRUE)

# output package information
if(NOT xf_voice_FIND_QUIETLY)
  message(STATUS "Found xf_voice: 0.0.0 (${xf_voice_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'xf_voice' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${xf_voice_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(xf_voice_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${xf_voice_DIR}/${_extra}")
endforeach()
