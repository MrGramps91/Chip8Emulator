//.h files contain definitions of Functions and variables, which are imported or un in any C++ program

#include<iostream>
#include"Opcodes.h"

class Chip8 {

private:
	unsigned short opcode; //Current opcode
	unsigned char memory[4096]; // memory 4k
	unsigned char V[16]; //CPU registers V1-VE
	unsigned char buffer[1184];  // buffer for loading a program into memory  
	unsigned short I; //Index register I 
	unsigned short pc; //Index register pc (program counter)
	unsigned char gfx[64 * 32]; //gfx has a total of 2048 pixels 
	unsigned char delay_timer;
	unsigned char sound_timer; //Sys buzzer sounds when the sound_timer reaches zero
	unsigned short stack[16]; // 16 level stack
	unsigned short sp; // stack pointer
	unsigned char key[16]; //stores current state of key
	bool drawFlag;
	void emulateCycle();
	void initialize();
	void loadGame();

};
#pragma once
