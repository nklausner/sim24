#pragma once
#include <string>
#include <vector>
#include <iostream>


//2022-06-17: initialize tile
//a tile should hold the rails, infra and pointers to trains in a region


class Point
{
public:
	Point(int x, int y, int function);
	int x = 0;
	int y = 0;
	int function = 0;
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

	int xt = 0;
	int yt = 0;
	std::vector<std::vector<Point>> rails = {};
	std::vector<std::vector<Pointf>> platforms = {};
	std::vector<Label> labels = {};
};