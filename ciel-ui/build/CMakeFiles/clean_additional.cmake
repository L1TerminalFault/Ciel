# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/ciel-ui_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ciel-ui_autogen.dir/ParseCache.txt"
  "CMakeFiles/ciel-uiplugin_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ciel-uiplugin_autogen.dir/ParseCache.txt"
  "ciel-ui_autogen"
  "ciel-uiplugin_autogen"
  "test/CMakeFiles/ciel-test_autogen.dir/AutogenUsed.txt"
  "test/CMakeFiles/ciel-test_autogen.dir/ParseCache.txt"
  "test/ciel-test_autogen"
  )
endif()
