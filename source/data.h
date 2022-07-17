#pragma once
#include <string>
#include <vector>
#include "tile.h"

//2022-06-16: lazy data storage


enum MyCommands
{
	TERMINATE,
	CONTINUE,
	SCROLL_UP,
	SCROLL_DO,
	SCROLL_LE,
	SCROLL_RI,
	ZOOM_IN,
	ZOOM_OUT
};


namespace data
{

	extern int command;	//simulation command
	extern int ticks;	//simulation ticks
	extern int tpf;		//ticks per frame
	extern int fps;		//frames per second
	extern int speed;	//speed factor
	extern int time;	//simulation time in milliseconds

	extern const int s;	//tile size in meter

	extern const int zoomarr[5];
	extern int zoom;	//zoom factor in dezimeters per pixel

	extern int w;		//screen width in pixels
	extern int h;		//screen height in pixels
	extern int x;		//x location in meters
	extern int y;		//y location in meters

	extern int xl;		//border in meters (left)
	extern int xr;		//border in meters (right)
	extern int yu;		//border in meters (upper)
	extern int yd;		//border in meters (bottom)

	extern int xrel[24];
	extern int yrel[24];

	extern std::vector<Tile> tiles;

	extern const std::string my_directory;
	extern const std::vector<std::string> infra_files;

	extern int total_rails;
	extern int total_platforms;
	extern int total_labels;
	extern int drawn_rails;
	extern int drawn_platforms;
	extern int drawn_labels;
}


Tile* find_tile(int x, int y, bool create_new);
int get_tile_index(int x);