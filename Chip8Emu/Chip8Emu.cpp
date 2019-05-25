// Chip8Emu.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include "GL/glut.h" //Graphics and input
#include "Chip8.h" // Assigned variables

unsigned char chip8_fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20 ,0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10 ,0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40 ,0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90 ,0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90 ,0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80, //E
};


void Chip8::initialize()
{
	//initialize registers and memory once
	pc = 0x200;     // Program counter starts at 0x200
	opcode = 0;         // Reset current opcode
	I = 0;         // Reset index register
	sp = 0;			// Reset stack pointer

	// Clear display
	// Clear stack
	// Clear registers V0-VF
	// Clear memory

	// Load fontset
	for (int i = 0; i < 80; i++)
		memory[i] = chip8_fontset[i];

	// Reset timers

}

void Chip8::loadGame()
{

	for (int i = 0; i < 1185; i++) //setting bufferSize to 1185 to prevent overflow and set buffer to 1184 bytes availble for Cpu instruction
		memory[i + 512] = buffer[i];
}

void Chip8::emulateCycle()
{
	//Fetch opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	//Decode opcode
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x00E0: // 0x00E0: Clears the screen
			// Execute opcode
			break;

		case  0x00EE: // 0x00EE: Returns from subroutine
			// Execute opcode
			break;

		default:
			printf("Unknow opcode [0x0000]: 0x%X\n", opcode);
		}
		break;

	case 0x2000: //0x2NNN calls subroutine at address NNN
		stack[sp] = pc;
		++sp;
		pc = opcode & 0x0FFF;
		break;

	case 0x0004: //0x8XY4  adds value of VY to VX
		if (V[opcode & 0x00F0 >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
			V[0xF] = 1; //Carry
		else
			V[0xF] = 0;
		V[(opcode & 0X0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
		pc += 2;
		break;

	case 0x0033: //store the Binary-coded representation of VX at the addresses I, I+1, I+2
		memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
		memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
		memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
		pc += 2;

	case 0xD000: // Drawing pixels/sprites at specific locations
	{
		unsigned short x = V[(opcode & 0x0F00) >> 8];
		unsigned short y = V[(opcode & 0x00F0) >> 4];
		unsigned short height = opcode & 0x000F;
		unsigned short pixel;
		bool drawFlag = false;

		V[0xF] = 0;
		for (int yline = 0; yline < height; yline++)
		{
			pixel = memory[I + yline];
			for (int xline = 0; xline < 8; xline++)
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					if (gfx[(x + xline + ((y + yline) * 64))] == 1)
						V[0xF] = 1;
					gfx[x + xline + ((y + yline) * 64)] ^= 1;
				}
			}
		}
		drawFlag = true;
		pc += 2;
	}
	break;

	case 0XE000:
	{
		//EX9E: Skips the next instruction
		// if the key stored in VX is pressed
	case 0x009E:
		if (key[V[(opcode & 0x0F00) >> 8]] != 0)
			pc += 4;
		else
			pc += 2;
		break;
	}
	// More opcode //


	case 0xA000: // ANNN: Sets I to the address NNN
		//Execute opcode	
		I = opcode & 0xFFF;
		pc += 2;
		break;

		// More opcodes //

	default:
		printf("Unknown opcode: 0x%X\n", opcode);
	}

	//Update timers
	if (delay_timer > 0)
		--delay_timer;

	if (sound_timer > 0)
	{
		if (sound_timer == 1)
			printf("Beep!\n");
		--sound_timer;
	}


}