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

};

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
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		w, h, SDL_WINDOW_SHOWN
	);

	if (window == NULL)
	{
		printf("Window could not be created! SDL_ERROR: %s\n", SDL_GetError());
		exit(2);
	}

	// Create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, w, h);

	// Create texture that stores frame buffer
	SDL_Texture* sdlTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

	// Temporary Pixel Buffer
	uint32_t pixels[2048];

	load:
	// Attempt to load ROM
	if (!chip8.loadGame(argv[1]))
		return 2;

	//Emulation loop
	while (true) 
	{
		chip8.emulateCycle();

		// Process SDL events
		SDL_Event e;
		while (SDL_PollEvent(&e)) 
		{
			if (e.type == SDL_QUIT) exit(0);

			// POCESS KEYDOWN EVENTS
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
					exit(0);
				if (e.key.keysym.sym == SDLK_F1)
					goto load; // used to reset/reload ROM

				for (int i = 0; i < 16; ++i)
				{
					if (e.key.keysym.sym == keymap[i])
					{
						chip8.key[i] = 1;
					}
				}
			}
			// Process keyup events
			if (e.type == SDL_KEYUP)
			{
				for (int i = 0; i < 16; i++)
				{
					if (e.key.keysym.sym == keymap[i])
					{
						chip8.key[i] = 0;
					}
				}
			}
		}

		// If draw occurred, redraw SDL screen
		if (chip8.drawFlag)
		{
			chip8.drawFlag = false;

			// Store pixels in a temporary buffer
			for (int i = 0; i < 2048; ++i)
			{
				uint8_t pixel = chip8.gfx[i];
				pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
			}

			// Update SDL Texture
			SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));

			//Clear screen and render
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		// Sleep to slow down emulation speed
		std::this_thread::sleep_for(std::chrono::microseconds(1200));
	}
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