#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>


//2022-07-07: initialize writer


struct SDL_Writer
{
public:
	SDL_Writer(SDL_Renderer* r);
	void write(std::string s, int x, int y);
	void write_center(std::string s, int x, int y);
	const int fontsize = 16;
	const int fontwide = 8;

private:
	SDL_Renderer* renderer = NULL;
	TTF_Font* charfont;
	SDL_Color charcolor;
	SDL_Surface* charsurf[256];
	SDL_Texture* chartext[256];

};