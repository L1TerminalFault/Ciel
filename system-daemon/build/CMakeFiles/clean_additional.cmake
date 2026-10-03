# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/ciel-system-daemon_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ciel-system-daemon_autogen.dir/ParseCache.txt"
  "ciel-system-daemon_autogen"
  )
endif()
