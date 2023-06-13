# CMakeList.txt : CMake project for ex4, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ex4 "presubmit.cpp" "Matrix.cpp" "Dense.cpp" "Activation.cpp" "MlpNetwork.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ex4 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
