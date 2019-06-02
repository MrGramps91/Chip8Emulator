//.h files contain definitions of Functions and variables, which are imported or un in any C++ program
#ifndef Chip_8_H
#define Chip_8_H

#include<stdint.h>

class Chip8 {

private:
	uint16_t  opcode; //Current opcode
	uint16_t  pc; //Index register pc (program counter)
	uint16_t  I; //Index register pc (program counter) I; //Index register I 

	uint16_t stack[16]; // 16 level stack
	uint16_t sp; // stack pointer

	uint8_t memory[4096]; // memory 4k
	uint8_t  V[16]; //CPU registers V1-VE

	uint8_t delay_timer;
	uint8_t  sound_timer; //Sys buzzer sounds when the sound_timer reaches zero

	void initialize();

public:
	
	unsigned char buffer[1184];  // buffer for loading a program into memory  
	uint8_t  gfx[64 * 32]; //gfx has a total of 2048 pixels 
	uint8_t key[16]; //stores current state of key
	bool drawFlag;

	Chip8();
	

	void emulateCycle();	
	bool loadGame(const char *file_path);

};
#endif

