# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/ciel-task-monitor_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ciel-task-monitor_autogen.dir/ParseCache.txt"
  "ciel-task-monitor_autogen"
  )
endif()
