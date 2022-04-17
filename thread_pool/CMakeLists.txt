# CMakeList.txt : CMake project for thread_pool, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
set(CMAKE_CXX_STANDARD 11)
add_library (thread_pool "thread_pool.cpp")
# TODO: Add tests and install targets if needed.
