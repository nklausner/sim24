#pragma once
#include <SDL.h>
#include "SDL_writer.h"
#include "data.h"
#include "tile.h"

//2022-06-16: create drawer

struct Drawer
{
public:
	Drawer(SDL_Renderer* renderer, SDL_Writer* mywriter);
	void update_coords();
	void increase_zoom();
	void decrease_zoom();
	void draw();

private:
	void draw_rails();
	void draw_platforms();
	void draw_labels();
	void draw_tile_grid();
	int get_tile_number(int x);
	void draw_line(Point& p0, Point& p1);
	void draw_line(Pointf& p0, Pointf& p1);

	SDL_Renderer* renderer = NULL;
	SDL_Writer* mywriter = NULL;
	int xt0;
	int xt1;
	int yt0;
	int yt1;
	std::vector<Tile*> active_tiles;
};