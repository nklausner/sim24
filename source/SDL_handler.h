#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include "data.h"


//2022-06-16: outsource SDL


struct SDL_handler
{
	SDL_handler();

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event;

	void close();
	int get_command();
	void cap_framerate();
};