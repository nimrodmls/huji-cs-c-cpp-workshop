# CMakeList.txt : CMake project for Cipher, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Cipher "main.c" "cipher.c" "cipher.h" "tests.c" "tests.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Cipher PROPERTY C_STANDARD 99)
endif()

# TODO: Add tests and install targets if needed.
