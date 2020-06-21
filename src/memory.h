#ifndef MEMORY_H
#define MEMORY_H

class MEMORY {
public:
	/* 8-bit, 4096 values for memory. 
	 * 0x000 - 0x1ff is reserved for interpreter.
	 * 0x200 - 0xfff is program/data space.
	 */
	unsigned char memory[4096] = {};
	unsigned short stack[16];

	unsigned short MAX_PROGRAM_SIZE;
	unsigned short PROGRAM_BEGINNING;
	unsigned short PROGRAM_END;

	MEMORY();
	~MEMORY();
	void initialize();
private:

};

#endif