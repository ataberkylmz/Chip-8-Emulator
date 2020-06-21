# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

cmake_minimum_required (VERSION 3.8)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

project ("Chip-8-Emu")

# Include sub-projects.
add_subdirectory ("external")
add_subdirectory ("src")

include_directories(
	external/glfw-3.3.2/include/
	external/glew-2.1.0/include/
	.
)

set(ALL_LIBS
	glfw
	GLEW
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)