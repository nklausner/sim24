#pragma once
#include <string>
#include <vector>
#include <map>
#include "tile.h"
#include "function.h"
#include "timetable.h"

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
	ZOOM_OUT,
	PAUSEUNPAUSE,
	SPEED_UP,
	SPEED_DOWN,
	ANALYZE_TILE
};


namespace data
{

	extern bool is_running;	//simulation status
	extern int command;	//simulation command
	extern int ticks;	//simulation ticks
	extern int tpf;		//ticks per frame
	extern int speed;	//speed factor, values 1-10
	extern int time;	//simulation time in milliseconds
	//extern int fps;		//frames per second
	//extern float speedfactor;	//speed factor in milliseconds

	extern const int s;	//tile size in meters
	extern const int tld; //train length for drawing in meters

	extern const int zoomarr[5];
	extern int zoom;	//zoom factor in dezimeters per pixel

	extern int w;		//screen width in pixels
	extern int h;		//screen height in pixels
	extern int x;		//x center location in meters
	extern int y;		//y center location in meters

	extern int xl;		//border in meters (left)
	extern int xr;		//border in meters (right)
	extern int yu;		//border in meters (upper)
	extern int yd;		//border in meters (bottom)

	extern int tldxl;	//border for train drawing in meters (left)
	extern int tldxr;	//border for train drawing in meters (right)
	extern int tldyu;	//border for train drawing in meters (upper)
	extern int tldyd;	//border for train drawing in meters (bottom)

	extern std::vector<Tile> tiles;
	extern std::vector<Timetable> timetables;

	extern const std::string my_directory;

	extern std::map<std::string, float> series_length;

	extern int total_rails;
	extern int total_platforms;
	extern int total_labels;
	extern int total_functions;
	extern int drawn_rails;
	extern int drawn_platforms;
	extern int drawn_labels;
	extern int drawn_switches;

	extern bool is_info;	//debugging status info
	extern bool is_debug;	//debugging status debug


	extern unsigned testcount;
}


Tile* find_tile(int x, int y, bool create_new);
int get_tile_index(int x);