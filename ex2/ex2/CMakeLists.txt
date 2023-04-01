# CMakeList.txt : CMake project for ex2, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ex2 "main.c" "sort_bus_lines.c" "test_bus_lines.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ex2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
