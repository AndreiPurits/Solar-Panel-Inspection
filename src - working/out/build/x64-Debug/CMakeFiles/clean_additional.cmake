# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ERIS_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ERIS_autogen.dir\\ParseCache.txt"
  "ERIS_autogen"
  )
endif()
