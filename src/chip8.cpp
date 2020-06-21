#include <iostream>
#include "system.h"

int main() {

	SYSTEM* system = new SYSTEM();
	system->initialize();
	system->load_rom((char*)"PONG");
	//system->load_rom((char*)"D:\\Projects\\Chip-8-Emu\\Chip-8-Emu\\PONG");

	/*
	while (system->cpu->pc != 0xfff) {
		system->cpu->parse_instruction(system->memory);
	}
	*/
	
	system->cpu->parse_instruction(system->memory);
	system->cpu->parse_instruction(system->memory);
	system->cpu->parse_instruction(system->memory);
	system->cpu->parse_instruction(system->memory);
	

	GLFWwindow* window;

	if(!glfwInit()) return -1;

	window = glfwCreateWindow(500, 500, "Chip-8 Emu", NULL, NULL);

	if(!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if(!glewInit() == GLEW_OK) return -1;

	while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

	delete system;
	
  glfwTerminate();

	return 0;
}