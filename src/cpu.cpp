#include <iostream>
#include <ctime>
#include <cstdlib>
#include "cpu.h"
#include "system.h"
#include "memory.h"

void CPU::initialize() {
	// Set the location of Program Counter 
	pc = 0x200;
	sp = 0x0;
	I = 0x0;

	for (int i = 0; i < 16; i++)
		v[i] = 0;
}

void CPU::parse_instruction(MEMORY* memory) {
	// Instructions are 2 bytes long
	unsigned short instruction = memory->memory[pc] << 8;
	pc++;
	instruction |= memory->memory[pc];
	pc++;

	std::cout << "************************************" << std::endl;

	unsigned char command = instruction >> 12;
	std::cout << "Command: " << (int)command << std::endl;
	
	switch (command)
	{
	case 0x1: {
		// Instrction that changes the PC(program counter) to a specified location a.k.a a jump instrction.
		// Template: 1NNN, where NNN is the new location/value of the PC.
		
		unsigned short new_addr = instruction & 0x0fff;
		
		unsigned short beginning = memory->PROGRAM_BEGINNING;
		unsigned short end = memory->PROGRAM_END;

		if (new_addr >= memory->PROGRAM_BEGINNING && new_addr <= memory->PROGRAM_END) {
			std::cout << "PC is set to address " << (int)new_addr << std::endl;
			pc = new_addr;
		}
		else {
			std::cout << "Register address error. Acceptable address range is 0x200 - 0xfff, got: " << (int)new_addr << std::endl;
		}

		break;
	}
	case 0x2: {
		// Instrction that calls subroutine.
		// Template: 2NNN, where NNN is the location of subroutine.
		// To call a subroutine, put the current PC into stack, increment the stack pointer, then change the value of PC to NNN.
		
		unsigned short new_addr = instruction & 0x0fff;

		if (new_addr >= memory->PROGRAM_BEGINNING && new_addr <= memory->PROGRAM_END) {
			std::cout << "Calling the subroutine at address " << (int)new_addr << std::endl;
			memory->stack[sp] = pc;
			sp++;
			pc = new_addr;
		}
		else {
			std::cout << "Register address error. Acceptable address range is 0x200 - 0xfff, got: " << (int)new_addr << std::endl;
		}
		
		break;
	}
	case 0x3: {
		// Instruction that skips the next instruction if the condition is met.
		// Template: 3xKK, where x is the selected register from 0x0 to 0xf, KK is the value to check inside the register.
		
		unsigned char reg_addr = (instruction & 0x0f00) >> 8;

		if (reg_addr <= 0xF && reg_addr >= 0x00) {
			std::cout << "Skip instruction has been called to register V" << (int)reg_addr << std::endl;

			unsigned char value = instruction & 0x00ff;
			std::cout << "V" << (int)reg_addr << "= " << (int)value << std::endl;
			
			if (v[reg_addr] == value) {
				std::cout << "Condition is met, skipping the next instruction." << std::endl;
				pc += 2;
			}
			else {
				std::cout << "Condition is not met, next instruction will not be skipped." << std::endl;
			}
		}
		else {
			std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr << std::endl;
		}
		break;
	}
	case 0x4: {
		// Instruction that skips the next instruction if the condition is NOT met.
		// Template: 4xKK, where x is the selected register from 0x0 to 0xf, KK is the value to check inside the register.
		
		unsigned char reg_addr = (instruction & 0x0f00) >> 8;

		if (reg_addr <= 0xF && reg_addr >= 0x00) {
			std::cout << "Skip if not instruction has been called to register V" << (int)reg_addr << std::endl;

			unsigned char value = instruction & 0x00ff;
			std::cout << "V" << (int)reg_addr << "= " << (int)value << std::endl;

			if (v[reg_addr] != value) {
				std::cout << "Condition is not met, skipping the next instruction." << std::endl;
				pc += 2;
			}
			else {
				std::cout << "Condition is met, next instruction will not be skipped." << std::endl;
			}
		}
		else {
			std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr << std::endl;
		}
		break;
	}
	case 0x5: {
		// Instruction that skips the next instruction if the two registers are containing the same value.
		// Template: 5xy0, where x and y are the selected registers from 0x0 to 0xf.
		
		unsigned char sub_selection = instruction & 0x000f;
		
		if (sub_selection == 0x0) {
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;
			unsigned char reg_addr_y = (instruction & 0x00f0) >> 4;

			if ((reg_addr_x <= 0xF && reg_addr_x >= 0x00) && (reg_addr_y <= 0xF && reg_addr_y >= 0x00)) {
				std::cout << "Skip if registers are equal instruction has been called to register V" << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;

				if (v[reg_addr_x] == v[reg_addr_y]) {
					std::cout << "Condition is met, skipping the next instruction." << std::endl;
					pc += 2;
				}
				else {
					std::cout << "Condition is not met, next instruction will not be skipped." << std::endl;
				}
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
			}
		}
		else {
			std::cout << "Invalid instruction 5xy" << sub_selection << std::endl;
		}

		break;
	}
	case 0x6: {
		// Instruction that loads a value into V.
		// Template: 6xKK, where x is the selected register from 0x0 to 0xf, KK is the value to be loaded.
		
		unsigned char reg_addr = (instruction & 0x0f00) >> 8;

		if (reg_addr <= 0xF && reg_addr >= 0x00) {
			std::cout << "Load instruction has been called to register V" << (int)reg_addr << std::endl;

			unsigned char value = instruction & 0x00ff;
			std::cout << "V" << (int)reg_addr << "= " << (int)value << std::endl;
			v[reg_addr] = value;
		}
		else {
			std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr << std::endl;
		}

		break;
	}
	case 0x7: {
		// Instruction that adds a value into selected register and store it in that selected register.
		// Template: 7xKK, where x is the selected register from 0x0 to 0xf, KK is the value to be added.
		
		unsigned char reg_addr = (instruction & 0x0f00) >> 8;

		if (reg_addr <= 0xF && reg_addr >= 0x00) {
			std::cout << "Add instruction has been called to register V" << (int)reg_addr << std::endl;

			unsigned char value = instruction & 0x00ff;
			v[reg_addr] += value;
		}
		else {
			std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr << std::endl;
		}

		break;
	}
	case 0x8: {
		// INSTRUCTINO WITH SUB SELECTIONS.

		unsigned char sub_selection = instruction & 0x000f;

		if (sub_selection == 0x0) {
			// Instruction that stores a value of a register in another register, a.k.a copies it.
			// Template: 8xy0, where x and y are the selected registers from 0x0 to 0xf.
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;
			unsigned char reg_addr_y = (instruction & 0x00f0) >> 4;

			if ((reg_addr_x <= 0xF && reg_addr_x >= 0x00) && (reg_addr_y <= 0xF && reg_addr_y >= 0x00)) {
				std::cout << "V[x] = V[y] instruction has been called to register V" << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;

				v[reg_addr_x] = v[reg_addr_y];
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
			}
		}
		else if (sub_selection == 0x1) {
			// Instruction that performs a bitwise OR with another register.
			// Template: 8xy1, where x and y are the selected registers from 0x0 to 0xf. v[x] = v[x] OR v[y].
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;
			unsigned char reg_addr_y = (instruction & 0x00f0) >> 4;

			if ((reg_addr_x <= 0xF && reg_addr_x >= 0x00) && (reg_addr_y <= 0xF && reg_addr_y >= 0x00)) {
				std::cout << "V[x] |= V[y] instruction has been called to register V" << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;

				v[reg_addr_x] |= v[reg_addr_y];
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
			}
		}
		else if (sub_selection == 0x2) {
			// Instruction that performs a bitwise AND with another register.
			// Template: 8xy2, where x and y are the selected registers from 0x0 to 0xf. v[x] = v[x] AND v[y].
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;
			unsigned char reg_addr_y = (instruction & 0x00f0) >> 4;

			if ((reg_addr_x <= 0xF && reg_addr_x >= 0x00) && (reg_addr_y <= 0xF && reg_addr_y >= 0x00)) {
				std::cout << "V[x] &= V[y] instruction has been called to register V" << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;

				v[reg_addr_x] &= v[reg_addr_y];
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
			}
		}
		else if (sub_selection == 0x3) {
			// Instruction that performs a bitwise XOR with another register.
			// Template: 8xy3, where x and y are the selected registers from 0x0 to 0xf. v[x] = v[x] XOR v[y].
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;
			unsigned char reg_addr_y = (instruction & 0x00f0) >> 4;

			if ((reg_addr_x <= 0xF && reg_addr_x >= 0x00) && (reg_addr_y <= 0xF && reg_addr_y >= 0x00)) {
				std::cout << "V[x] ^= V[y] instruction has been called to register V" << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;

				v[reg_addr_x] ^= v[reg_addr_y];
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
			}
		}
		else if (sub_selection == 0x4) {
			// Instruction that performs a bitwise ADD with another register, set VF to 1 if carry bit presents.
			// Template: 8xy4, where x and y are the selected registers from 0x0 to 0xf. v[x] = v[x] + v[y], carry bit? VF=1: VF=0.
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;
			unsigned char reg_addr_y = (instruction & 0x00f0) >> 4;

			if ((reg_addr_x <= 0xF && reg_addr_x >= 0x00) && (reg_addr_y <= 0xF && reg_addr_y >= 0x00)) {
				std::cout << "V[x] += V[y] instruction has been called to register V" << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
				unsigned short value = v[reg_addr_x] + v[reg_addr_y];

				unsigned char carry = value & 0xff00 >> 8;
				value &= 0x00ff;
				v[reg_addr_x] = value;

				if (carry > 0) {
					v[0xf] = 1;
				}
				else {
					v[0xf] = 0;
				}
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
			}
		}
		else if (sub_selection == 0x5) {
			// TODO: Further investigation needed!
			// Instruction that performs a SUBTRACT operation with another register, set VF to 1 if v[x] > v[y], else 0.
			// Template: 8xy5, where x and y are the selected registers from 0x0 to 0xf. v[x] = v[x] - v[y], v[x] > v[y]? VF=1: VF=0.
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;
			unsigned char reg_addr_y = (instruction & 0x00f0) >> 4;

			if ((reg_addr_x <= 0xF && reg_addr_x >= 0x00) && (reg_addr_y <= 0xF && reg_addr_y >= 0x00)) {
				std::cout << "V[x] -= V[y] instruction has been called to register V" << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;

				if (v[reg_addr_x] > v[reg_addr_y]) {
					v[0xf] = 1;
				}
				else {
					v[0xf] = 0;
				}

				v[reg_addr_x] = v[reg_addr_x] - v[reg_addr_y];
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
			}
		}
		else if (sub_selection == 0x6) {
			// Instruction that performs a bitwise SHIFT RIGHT operation on v[x], set VF to 1 if least significant bit is 1, else 0.
			// Template: 8xy6, where x is the selected register from 0x0 to 0xf. v[x] = v[x] >> 1.
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;

			if (reg_addr_x <= 0xF && reg_addr_x >= 0x00) {
				std::cout << "V[x] = V[x] >> 1 instruction has been called to register V" << (int)reg_addr_x  << std::endl;

				unsigned char least_significant_bit = v[reg_addr_x] & 0x1;
				
				if (least_significant_bit == 1) {
					v[0xf] = 1;
				}
				else {
					v[0xf] = 0;
				}

				v[reg_addr_x] = v[reg_addr_x] >> 1;
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << std::endl;
			}
		}
		else if (sub_selection == 0x7) {
			// TODO: Further investigation needed!
			// Instruction that performs a SUBTRACT operation with another register, set VF to 1 if v[y] > v[x], else 0.
			// Template: 8xy7, where x and y are the selected registers from 0x0 to 0xf. v[x] = v[y] - v[x], v[y] > v[x]? VF=1: VF=0.
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;
			unsigned char reg_addr_y = (instruction & 0x00f0) >> 4;

			if ((reg_addr_x <= 0xF && reg_addr_x >= 0x00) && (reg_addr_y <= 0xF && reg_addr_y >= 0x00)) {
				std::cout << "V[x] = V[x] - V[y] instruction has been called to register V" << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;

				if (v[reg_addr_y] > v[reg_addr_x]) {
					v[0xf] = 1;
				}
				else {
					v[0xf] = 0;
				}

				v[reg_addr_x] = v[reg_addr_y] - v[reg_addr_x];
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
			}
		}
		else if (sub_selection == 0xE) {
			// Instruction that performs a bitwise SHIFT LEFT operation on v[x], set VF to 1 if least significant bit is 1, else 0.
			// Template: 8xyE, where x is the selected register from 0x0 to 0xf. v[x] = v[x] << 1.
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;

			if (reg_addr_x <= 0xF && reg_addr_x >= 0x00) {
				std::cout << "V[x] = V[x] << 1 instruction has been called to register V" << (int)reg_addr_x << std::endl;

				unsigned char least_significant_bit = v[reg_addr_x] & 0x1;

				if (least_significant_bit == 1) {
					v[0xf] = 1;
				}
				else {
					v[0xf] = 0;
				}

				v[reg_addr_x] = v[reg_addr_x] << 1;
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << std::endl;
			}
		}

		break;
	}
	case 0x9: {
		// Instruction that skips the next instruction if the two registers are not containing the same value.
		// Template: 9xy0, where x and y are the selected registers from 0x0 to 0xf.

		unsigned char sub_selection = instruction & 0x000f;

		if (sub_selection == 0x0) {
			unsigned char reg_addr_x = (instruction & 0x0f00) >> 8;
			unsigned char reg_addr_y = (instruction & 0x00f0) >> 4;

			if ((reg_addr_x <= 0xF && reg_addr_x >= 0x00) && (reg_addr_y <= 0xF && reg_addr_y >= 0x00)) {
				std::cout << "Skip if registers are not equal instruction has been called to register V" << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;

				if (v[reg_addr_x] != v[reg_addr_y]) {
					std::cout << "Condition is met, skipping the next instruction." << std::endl;
					pc += 2;
				}
				else {
					std::cout << "Condition is not met, next instruction will not be skipped." << std::endl;
				}
			}
			else {
				std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr_x << " and " << (int)reg_addr_y << std::endl;
			}
		}
		else {
			std::cout << "Invalid instruction 9xy" << sub_selection << std::endl;
		}

		break;
	}
	case 0xA: {
		// Instruction that loads a value into I.
		// Template: ANNN, NNN is the value to be loaded.

		//TODO: Does this need checking the loaded value? Register I is 'MOSTLY' used for storing a memory address.
		// So it needs to be between 0x000 and 0xfff, yet it is not enforced.
		unsigned short value = instruction & 0x0fff;
			
		I = value;

		break;
	}
	case 0xB: {
		// Instruction that jumps to NNN + v[0].
		// Template: BNNN, NNN is the address.

		unsigned short address = instruction & 0x0fff;
		unsigned short jump_address = address + v[0];

		if (jump_address >= 0x200 && jump_address <= 0xfff) {
			pc = jump_address;
		}
		else {
			std::cout << "Jump address " << (int)jump_address << " is out of boud (0x200 - 0xfff)." << std::endl;
		}

		break;
	}
	case 0xC: {
		// Instruction that sets v[x] to the result of random value (0-255) bitwise AND given value.
		// Template: CxKK, where x is the selected register from 0x0 to 0xf, KK is the value to be added. x[x] = rand & kk.

		unsigned char reg_addr = (instruction & 0x0f00) >> 8;

		if (reg_addr <= 0xF && reg_addr >= 0x00) {
			std::srand(std::time(nullptr));
			unsigned char rand = std::rand() % 256;

			std::cout << "Random instruction has been called to register V" << (int)reg_addr << std::endl;

			unsigned char value = instruction & 0x00ff;
			v[reg_addr] = rand & value;
		}
		else {
			std::cout << "Register address error. Acceptable address range is 0x0 - 0xf, got: " << (int)reg_addr << std::endl;
		}


	}
	case 0xD: {
		/*
		Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

		The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
		*/
		// TODO: implement this register after implementing screen via graphics library.
	}
	default:
		pc++;
		break;
	}
}

CPU::CPU() {
	// Cpu constructor
}
CPU::~CPU() {
	// Cpu destructor
}