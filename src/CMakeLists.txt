# CMakeList.txt : CMake project for Chip-8-Emu, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package(OpenGL REQUIRED)

# Add source to this project's executable.
add_executable (Chip8 "chip8.cpp")
add_library(system STATIC "system.cpp" "system.h")
add_library(cpu STATIC "cpu.cpp" "cpu.h")
add_library(memory STATIC "memory.cpp" "memory.h")
target_link_libraries(Chip8 PUBLIC glfw ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES})
target_link_libraries(Chip8 PUBLIC GLEW ${GLEW_INCLUDE} ${GLEW_SOURCE})
target_link_libraries(Chip8
	${ALL_LIBS}
)
target_link_libraries(Chip8 PUBLIC system)
target_link_libraries(Chip8 PUBLIC cpu)
target_link_libraries(Chip8 PUBLIC memory)

# TODO: Add tests and install targets if needed.
configure_file(PONG PONG COPYONLY)