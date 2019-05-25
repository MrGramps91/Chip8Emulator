#include "Chip8.h" //Cpu core implementation
#include "Opcodes.h" //CPU opcode implementations
#include "SDL.h"

Chip8 emuChip8;


int main(int argc, char** argv)
{
	//set up render sys and register input callbacks
	setUpGraphics();
	setUpInput();

	//Initialize the Chip8 sys and load game into the memory
	emuChip8.initialize();
	emuChip8.loadGame("insert game name");

	//Emulation loop
	for (;;)
	{
		//Emulate one cycle
		emuChip8.emulateCycle();

		//if the draw flag is set update screen
		if (emuChip8.drawFlag)
			drawGraphics();

		//Store key press state (Press and Release)
		emuChip8.setKeys();
	}
	return 0;

}