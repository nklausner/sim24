#include "data.h"

namespace data
{

	bool is_running = true;
	int command = CONTINUE;
	int ticks = 0;
	int tpf = 50;
	int speed = 2;
	int time = 43200000; // + 15 * 60000; //= 12:00:00
	//int fps = 1000 / tpf;
	//float speedfactor = speed / float(1000.0);

	const int s = 2000;
	const int tld = 500;

	const int zoomarr[5] = { 1000, 200, 50, 10, 2 }; //= dm/pix
	int zoom = 10;

	int w = 1700;
	int h = 960;
	int x = -20200;
	int y = +23500;
	//-20200,+23500 Herrenberg
	//+7500,-4800 Enkheim

	int xl = x - w * zoom / 20;
	int xr = x + w * zoom / 20;
	int yu = y - h * zoom / 20;
	int yd = y + h * zoom / 20;

	int tldxl = xl - tld;
	int tldxr = xr + tld;
	int tldyu = yu - tld;
	int tldyd = yd + tld;


	std::vector<Tile> tiles = {};
	std::vector<Timetable> timetables = {};

	const std::string my_directory = "D:\\Construction\\";

	std::map<std::string, float> series_length = {};

	int total_rails = 0;
	int total_platforms = 0;
	int total_labels = 0;
	int total_functions = 0;
	int drawn_rails = 0;
	int drawn_platforms = 0;
	int drawn_labels = 0;
	int drawn_switches = 0;

	bool is_info = true;
	bool is_debug = true;

	unsigned testcount = 0;
}


Tile* find_tile(int x, int y, bool create_new)
{
	//find tile or create new tile
	//return tile reference
	for (Tile& t : data::tiles)
	{
		if (t.xt == x && t.yt == y) {
			return &t;
		}
	}
	if (create_new) {
		Tile new_tile(x, y);
		data::tiles.push_back(new_tile);
		return &data::tiles.back();
	}
	return NULL;
}


int get_tile_index(int x)
{
	return (x < 0) ? (x / data::s) - 1 : x / data::s;
}