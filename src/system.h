#include "cpu.h"
#include "memory.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class SYSTEM {
public:
	SYSTEM();
	~SYSTEM();
	void initialize();
	void load_rom(char*);

	CPU* cpu;
	MEMORY* memory;

private:
	
};

