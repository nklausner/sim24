#include "SDL_writer.h"


SDL_Writer::SDL_Writer(SDL_Renderer* r)
{
	//std::cout << "debug debug debug debug" << std::endl;
	renderer = r;

	if (TTF_Init() >= 0)
	{
		//open font style and set size
		std::string s = "";
		//charfont = TTF_OpenFont("D:\\Python\\vclib\\SDL2_ttf-2.20.1\\fonts\\consolas\\CONSOLA.ttf", fontsize);
		charfont = TTF_OpenFont("D:\\Python\\vclib\\SDL2_ttf-2.0.18\\fonts\\clacon2.ttf", fontsize);
		charcolor = { 0,0,0 };
		for (unsigned i = 0; i < 256; i++)
		{
			s = i;
			charsurf[i] = TTF_RenderText_Solid(charfont, s.c_str(), charcolor);
			if (!charsurf[i])
			{
				charsurf[i] = TTF_RenderText_Solid(charfont, " ", charcolor);
				//std::cout << "error: ";
			}
			chartext[i] = SDL_CreateTextureFromSurface(renderer, charsurf[i]);
			//std::cout << s.c_str() << "  " << i << std::endl;
		}
		std::cout << TTF_GetError() << std::endl;
	}
	else
	{
		//Error handling SDL
		std::cout << "Could not initialize SDL_ttf " << TTF_GetError() << std::endl;

	}
	return;
}


void SDL_Writer::write(std::string mystr, int x, int y)
{
	//std::cout << "sdl writer test test" << std::endl;
	for (unsigned i = 0; i < mystr.size(); i++)
	{
		int j = (mystr[i] + 256) % 256;
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = charsurf[j]->w;
		r.h = charsurf[j]->h;
		SDL_RenderCopy(renderer, chartext[j], NULL, &r);
		x += r.w;
		//std::cout << mystr[i] << "  " << j << std::endl;
	}
	return;
}


void SDL_Writer::write_center(std::string mystr, int x, int y)
{
	write(mystr, x - ((int)mystr.size() * fontwide / 2), y);
	return;
}