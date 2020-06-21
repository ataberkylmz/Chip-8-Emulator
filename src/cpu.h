#ifndef CPU_H
#define CPU_H
#include "memory.h"

class CPU {
public:
	// 8-bit Data registers V0 to VF.
	unsigned char v[16];

	// Variable that holds current opCode.
	unsigned short opCode;


	// Address register. Memory is 4096 bytes (0x0fff) so, a 2 byte address register would be sufficient.
	unsigned short I;

	// Program counter register. Same as address register.
	unsigned short pc;

	// Stack Pointer register. Stack got 16 16-bit values. A byte is sufficient enough to address topmost location of the stack.
	unsigned char sp;

	// 8-bit Delay and sound register
	unsigned char delay_register;
	unsigned char sound_register;

	CPU();
	~CPU();
	void initialize();
	void parse_instruction(MEMORY*);
private:
	
};

#endif