#include <iostream>
#include <chrono>
#include <thread>
#include "stdint.h"
#include "Chip8.h" //Cpu core implementation
#include "SDL.h"

Chip8 emuChip8;

using namespace std;

uint8_t keymap[16] = {
	SDLK_x,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_z,
	SDLK_c,
	SDLK_4,
	SDLK_r,
	SDLK_f,
	SDLK_v,
	
}

int main(int argc, char **argv)
{
	// Command usage
	if (argc != 2) 
	{
		cout << "Usage: Chip8 <ROM file>" << endl;
		return 1;
	}

	Chip8 chip8 = Chip8();  //initialize Chip8

	int w = 1024;	// Window width
	int h = 512;	// Window height

	// The window will be rendering to
	SDL_Window *window = NULL;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_ERROR: %s\n", SDL_GetError());
		exit(1);
	}

	//Create Window
	window = SDL_CreateWindow(
		"CHIP-8 Emulator",
		SDL_WINDOW_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		w, h, SDL_WINDOW_SHOWN
	);

	if (window == NULL)
	{
		printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
		exit(2);
	}

	// Create renderer

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, w, h);

}


//
////set up render sys and register input callbacks
//setUpGraphics();
//setUpInput();
//
////Initialize the Chip8 sys and load game into the memory
//emuChip8.initialize();
//emuChip8.loadGame("insert game name");
//
////Emulation loop
//for (;;)
//{
//	//Emulate one cycle
//	emuChip8.emulateCycle();
//
//	//if the draw flag is set update screen
//	if (emuChip8.drawFlag)
//		drawGraphics();
//
//	//Store key press state (Press and Release)
//	emuChip8.setKeys();
//}
//return 0;