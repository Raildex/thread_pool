# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("thread_pool")

# Include sub-projects.
add_subdirectory ("thread_pool")

add_executable(thread_pool_exe main.cpp)
target_include_directories(thread_pool_exe PRIVATE "./thread_pool")
target_link_libraries(thread_pool_exe thread_pool)
