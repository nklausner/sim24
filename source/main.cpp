/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/


#pragma once
#include "SDL_handler.h"
#include "SDL_writer.h"
#include "loader.h"
#include "drawer.h"
#include "train.h"
#include "data.h"
#include "geometry.h"
#include "trainManager.h"
#include "infoPanel.h"
#include <windows.h>


int main(int argc, char* args[])
{
	//fix console window position
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 60, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	//instanciate objects
	SDL_handler sdlh;
	SDL_Writer sdlw(sdlh.renderer);
	Loader loader;
	Drawer drawer(sdlh.renderer, &sdlw);
	InfoPanel infoPanel(sdlh.renderer, &sdlw);
	create_full_circle(data::xrel, data::yrel);

	//prepare stuff
	loader.load_all();
	drawer.update_coords();

	//initiate trains, depends on rails
	TrainManager trainmanager(sdlh.renderer, &sdlw);

	//run main loop handling user requests
	while (data::command)
	{
		data::time += data::tpf * data::speed;
		data::command = sdlh.get_command();
		if (data::command >= 2 && data::command < 8) {
			drawer.update_coords();
		}
		trainmanager.update();
		drawer.draw();
		trainmanager.draw_all();
		infoPanel.draw();

		SDL_RenderPresent(sdlh.renderer);
		sdlh.cap_framerate();

		//std::cout << "\r" << "drawn rails " << data::drawn_rails << "...." << std::flush;
		//std::cout << "\r" << "position " << data::x << "," << data::y << std::flush;
		//std::cout << "\r" << write_time() << std::flush;
	}

	sdlh.close();
	return 0;
}