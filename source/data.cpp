#include "data.h"

namespace data
{

	int command = CONTINUE;
	int ticks = 0;
	int tpf = 25;
	int fps = 40;
	int speed = 1;
	int time = 43200000;

	const int s = 1024;

	const int zoomarr[5] = { 1000, 200, 50, 10, 2 }; //= dm/pix
	int zoom = 10;

	int w = 1620;
	int h = 960;
	int x = 7500;
	int y = -4800;

	int xl = x - w * zoom / 20;
	int xr = x + w * zoom / 20;
	int yu = y - h * zoom / 20;
	int yd = y + h * zoom / 20;

	extern int xrel[24] = { 0 };
	extern int yrel[24] = { 0 };

	std::vector<Tile> tiles = {};

	const std::string my_directory = "D:\\Python\\sim\\";
	const std::vector<std::string> infra_files =
	{
		"A-Strecke.svg",
		"B-Strecke.svg",
		"C-Strecke.svg"
	};

	int total_rails = 0;
	int total_platforms = 0;
	int total_labels = 0;
	int drawn_rails = 0;
	int drawn_platforms = 0;
	int drawn_labels = 0;
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