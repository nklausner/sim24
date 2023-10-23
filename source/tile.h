#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "function.h"


//2022-06-17: initialize tile
//a tile should hold the rails, infra and pointers to trains in a region


class Point
{
public:
	Point(int x_, int y_, size_t i_);
	int x = 0;
	int y = 0;
	size_t findex = 4294967295; //index of tile->functions
};


class Pointf
{
public:
	Pointf(float x, float y);
	float x = 0;
	float y = 0;
};


class Label
{
public:
	Label(int x, int y, std::string s);
	int x = 0;
	int y = 0;
	std::string s = "";
};


class Tile
{
public:
	Tile(int xtn, int ytn);
	void add_initial_rail(int xn, int yn);
	void add_to_last_or_new_rail(int xn, int yn);

	int xt = 0;
	int yt = 0;
	std::vector<std::vector<Point>> rails = {};
	std::vector<std::vector<Pointf>> platforms = {};
	std::vector<Label> labels = {};
	std::vector<Function> functions = {};
};