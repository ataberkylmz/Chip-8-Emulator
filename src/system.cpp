#include "system.h"
#include "cpu.h"
#include "memory.h"
#include <iostream>
#include <fstream>

SYSTEM::SYSTEM() {
	cpu = new CPU();
	memory = new MEMORY();
}

SYSTEM::~SYSTEM() {
	// Destructor
	delete cpu;
	delete memory;
}

void SYSTEM::initialize() {
	cpu->initialize();
	memory->initialize();
}

void SYSTEM::load_rom(char* rom_name) {
	std::ifstream rom_file;
	rom_file.open(rom_name, std::ios::in | std::ios::binary | std::ios::ate);

	if (rom_file.is_open()) {
		short rom_size = rom_file.tellg();

		if (rom_size <= memory->MAX_PROGRAM_SIZE) {
			std::cout << "Rom size is " << (int)rom_size << " bytes" << std::endl;
			rom_file.seekg(0, std::ios::beg);
			rom_file.read((char*)(memory->memory + memory->PROGRAM_BEGINNING), rom_size);
		}
		else {
			std::cout << "Rom file is bigger than the memory of the system." << std::endl;
			std::cout << "Max rom size allowed: " << memory->MAX_PROGRAM_SIZE << " bytes" << std::endl;
			std::cout << "Current rom size: " << (int)rom_size << " bytes" << std::endl;
		}

		rom_file.close();
	}
	else {
		std::cout << "Cannot open file " << rom_name << std::endl;
	}
}