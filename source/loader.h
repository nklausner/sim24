#pragma once
#include <iostream>
#include <fstream>
#include <tuple>
#include "data.h"
#include "geometry.h"
#include "fileHandler.h"
//#include "switchBuilder.h"

//2022-06-12: create file handler

struct Loader
{
public:
	Loader();

private:
	bool extract_infra(std::vector<std::string>& my_raw);

	bool extract_rails(std::vector<std::string>& my_raw);
	void fill_tiles(std::vector<int>& my_vec);
	Tile* interpolate_points(int x1, int y1, int x0, int y0, int xt1, int yt1, int xt0, int yt0, Tile* tlast);
	int correct_interpolation(int x, int x0, int x1, int y0, int y1);
	bool found_point(std::vector<std::vector<int>>& my_vec, int x, int y, bool& found_double);
	std::tuple<int, int> find_next_bigger_point(std::vector<std::vector<int>>& my_vec, int l);
	void print_interpolation_vector(std::vector<std::vector<int>>& my_vec);


	void extract_label(std::string& my_line);
	void extract_platf(std::string& my_line);
	void fill_tiles_platforms(std::vector<float> my_vec);

	bool is_corrected;
};