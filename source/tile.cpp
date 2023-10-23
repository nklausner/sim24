#include "tile.h"


Point::Point(int x_, int y_, size_t i_)
{
	x = x_;
	y = y_;
	findex = i_;
	return;
}


Pointf::Pointf(float x_, float y_)
{
	x = x_;
	y = y_;
	return;
}


Label::Label(int x_, int y_, std::string s_)
{
	x = x_;
	y = y_;
	s = s_;
	return;
}


Tile::Tile(int xtn, int ytn)
{
	xt = xtn;
	yt = ytn;
	std::vector<std::vector<Point>> rails = {};
	std::vector<std::vector<Pointf>> platforms = {};
	std::vector<Label> labels = {};
	std::vector<Function> functions = {};

	//std::cout << "new tile " << xt << "," << yt << std::endl;
	return;
}


void Tile::add_initial_rail(int xn, int yn)
{
	//add to new vectors
	std::vector<Point> v = { Point(xn, yn, 4294967295) };
	rails.push_back(v);
	return;
}


void Tile::add_to_last_or_new_rail(int xn, int yn)
{
	//does nothing
	return;
}