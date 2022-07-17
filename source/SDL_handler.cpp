#include "SDL_handler.h"


SDL_handler::SDL_handler()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		//Create window
		window = SDL_CreateWindow("sim24", 240, 60, data::w, data::h, SDL_WINDOW_SHOWN);

		if (window)
		{
			//Get renderer
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		}
		else
		{
			//Error handling window
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
			return;
		}

		//std::cout << window << " " << renderer << std::endl;
	}
	else
	{
		//Error handling SDL
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}
	data::ticks = SDL_GetTicks();
	return;
}


void SDL_handler::close()
{
	//Proper deallocation and cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
	SDL_Quit();
	return;
}


int SDL_handler::get_command()
{
	//return last received keyboard or mouse event
	if (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT) {
			return TERMINATE;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:		return SCROLL_UP;
			case SDLK_DOWN:		return SCROLL_DO;
			case SDLK_LEFT:		return SCROLL_LE;
			case SDLK_RIGHT:	return SCROLL_RI;
			case SDLK_PAGEDOWN:	return ZOOM_IN;
			case SDLK_PAGEUP:	return ZOOM_OUT;
			}
		}
	}
	return CONTINUE;
}


void SDL_handler::cap_framerate()
{
	//delay if elapsed time is smaller than given frame time
	int t = SDL_GetTicks() - data::ticks;
	if (t < data::tpf) {
		SDL_Delay(data::tpf - t);
	}
	data::ticks = SDL_GetTicks();
	return;
}