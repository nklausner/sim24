#pragma once
#include <SDL.h>
#include "SDL_writer.h"
#include "data.h"


//2022-07-17: initialize info panel


struct InfoPanel
{
public:
	InfoPanel(SDL_Renderer* renderer, SDL_Writer* writer);
	void draw();

private:
	SDL_Renderer* renderer;
	SDL_Writer* writer;
	std::string writeFloat(int x);
	std::string writeTime(int t);
};