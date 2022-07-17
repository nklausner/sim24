#pragma once
#include <iostream>
#include <fstream>
#include "data.h"
#include "geometry.h"

//2022-06-12: create file handler

struct Loader
{
public:
	bool load_all();

private:
	bool read_file(std::string& filename);
	bool extract_infra(std::vector<std::string>& my_raw);

	bool extract_rails(std::vector<std::string>& my_raw);
	void fill_tiles(std::vector<int>& my_vec);
	void interpolate_points(int x1, int y1, int x0, int y0, int xt1, int yt1, int xt0, int yt0);
	int correct_interpolation(int x, int x0, int x1, int y0, int y1);

	bool extract_platforms(std::vector<std::string>& my_raw);
	void fill_tiles_platforms(std::vector<float> my_vec);

	void extract_label(std::string& my_line);

	bool is_corrected;
};