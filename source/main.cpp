/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/


//2022-07-17: craeting github repository


#pragma once
#include "SDL_handler.h"
#include "SDL_writer.h"
#include "loader.h"
#include "drawer.h"
#include "train.h"
#include "data.h"
#include "geometry.h"
#include "trainManager.h"
#include "functionLoader.h"
#include "infoPanel.h"
#include "tileAnalyzer.h"
#include <windows.h>


int main(int argc, char* args[])
{

	//fix console window position
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 0, 60, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	//instanciate objects
	SDL_handler sdlh;
	SDL_Writer sdlw(sdlh.renderer);
	Drawer drawer(sdlh.renderer, &sdlw);
	InfoPanel infoPanel(sdlh.renderer, &sdlw);
	create_full_circle();

	//prepare stuff
	Loader loader;
	FunctionLoader functionLoader;
	drawer.update_coords();

	//instanicate additional tools
	TileAnalyzer tileAnalyzer;

	//initiate trains, depends on rails
	TrainManager trainmanager(sdlh.renderer, &sdlw);
	std::cout << "[INFO] loading complete" << std::endl;

	//run main loop handling user requests
	while (data::command)
	{
		data::command = sdlh.get_command();
		if (data::command >= 2 && data::command < 8)
		{
			drawer.update_coords();
		}
		else if (data::command == PAUSEUNPAUSE)
		{
			data::is_running = !data::is_running;
		}
		else if (data::command == SPEED_UP && data::speed < 10)
		{
			data::speed++;
		}
		else if (data::command == SPEED_DOWN && data::speed > 1)
		{
			data::speed--;
		}
		else if (data::command == ANALYZE_TILE)
		{
			tileAnalyzer.print_current_tile();
		}
		if (data::is_running)
		{
			data::time += data::tpf * data::speed;
			trainmanager.update();
		}
		drawer.draw();
		trainmanager.draw_all_trains();
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