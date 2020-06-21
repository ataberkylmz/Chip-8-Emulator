add_subdirectory (glfw-3.3.2)

include_directories(
	glfw-3.3.2/include/GLFW/
	glew-2.1.0/include/
)

### GLEW ###

set(GLEW_SOURCE
	glew-2.1.0/src/glew.c
)

set(GLEW_HEADERS
)

add_library(GLEW STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)