# CMakeList.txt : CMake project for ex5, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ex5 
"Movie.cpp" 
"User.cpp" 
"RecommendationSystem.cpp"
"print_main.cpp" 
"RecommendationSystemLoader.cpp" 
"UsersLoader.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ex5 PROPERTY CXX_STANDARD 14)
endif()

# TODO: Add tests and install targets if needed.
