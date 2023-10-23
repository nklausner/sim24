#include "loader.h"


Loader::Loader()
{
	std::string my_filename = "";
	std::vector<std::string> my_vec = {};
	std::vector<std::string> my_files = { "Stuttgart-S", "Würt-Mitte" };

	for (std::string fn : my_files)
	{
		my_filename = data::my_directory + "infrastructure\\" + fn + ".svg";
		my_vec = read_file(my_filename);
		if (!my_vec.empty())
		{
			extract_rails(my_vec);
			extract_infra(my_vec);
		}
	}
	if (true)
	{
		my_filename = data::my_directory + "infrastructure\\stations.svg";
		my_vec = read_file(my_filename);
		if (!my_vec.empty())
		{
			extract_infra(my_vec);
		}
	}
	//print debug info
	if (data::is_info)
	{
		std::cout << "[INFO] tiles: " << data::tiles.size() << std::endl;
		std::cout << "[INFO] rails: " << data::total_rails << std::endl;
		std::cout << "[INFO] platfs: " << data::total_platforms << std::endl;
		std::cout << "[INFO] labels: " << data::total_labels << std::endl;
	}
	return;
}


bool Loader::extract_infra(std::vector<std::string>& my_raw)
{
	int my_modus = 0;
	for (std::string my_line : my_raw)
	{
		if (my_modus == 0)
		{
			if (my_line == "<g id=\"rails_simple\">") { my_modus = 1; }
			if (my_line == "<g id=\"stats_platf\">") { my_modus = 2; }
			if (my_line == "<g id=\"stats_label\">") { my_modus = 2; }
			if (my_line == "<g id=\"stations\">") { my_modus = 2; }
			continue;
		}
		if (my_modus > 0 && my_line == "</g>")
		{
			//std::cout << my_modus << std::endl;
			my_modus = 0;
			continue;
		}
		if (my_modus == 2 && my_line.length() > 10)
		{
			if (my_line.substr(0, 5) == "<path") { extract_platf(my_line); }
			if (my_line.substr(0, 5) == "<text") { extract_label(my_line); }
		}
	}
	return true;
}


bool Loader::extract_rails(std::vector<std::string>& my_raw)
{
	//extract coords from file
	std::vector<int> my_vec = {};
	std::string my_num = "";
	bool include = false;

	for (std::string& my_line : my_raw)
	{
		if (my_line == "</g>")
		{
			include = false;
		}
		if (include && my_line.length() > 6 && my_line.substr(0, 5) == "<path")
		{
			my_line.erase(0, 11);
			my_line.erase(my_line.length() - 4);
			my_line.push_back(' ');

			for (char c : my_line)
			{
				if (c != ' ' && c != ',') {
					my_num.push_back(c);
				}
				else if (my_num != "") {
					//std::cout << my_num << std::endl;
					my_vec.push_back( std::stoi(my_num) );
					my_num = "";
				}
			}
			fill_tiles(my_vec);
			my_vec.clear();
		}
		if (my_line == "<g id=\"rails_simple\">")
		{
			include = true;
		}
	}
	return true;
}


void Loader::fill_tiles(std::vector<int>& my_vec)
{
	//draw rail segments to tiles
	//handle crossing of tile borders --- TODO: if it starts with crossing
	int x1 = 0, y1 = 0;
	int x0 = 0, y0 = 0;
	int xt1 = 0, yt1 = 0;
	int xt0 = 0, yt0 = 0;
	Tile* t0 = NULL;
	Tile* t1 = NULL;
	for (unsigned i = 0; i < my_vec.size(); i += 2)
	{
		x1 = my_vec[i];
		y1 = my_vec[i+1];
		xt1 = get_tile_index(x1);
		yt1 = get_tile_index(y1);
		t1 = find_tile(xt1, yt1, true);

		//debugging jump points
		//if (x0 == 0 && y0 == 3400) {
		//	std::cout << "[DEBUG] " << x1 << "," << y1 << std::endl;
		//	std::cout << "[DEBUG] tiles " << t0 << " " << t1 << std::endl;
		//}

		if (t0 == NULL) //no point before
		{
			//std::cout << "add first point  " << x1 << "," << y1 << " tile " << xt1 << "," << yt1 << std::endl;
			t1->add_initial_rail(x1, y1);
			t0 = t1;
		}
		else if (t1 == t0)
		{
			//std::cout << "add follow point  " << x1 << "," << y1 << " tile " << xt1 << "," << yt1 << std::endl;
			t1->rails.back().push_back(Point(x1, y1, 4294967295));
			data::total_rails++;
		}
		else if (t1 != t0)
		{
			//std::cout << "add interpolation " << x1 << "," << y1 << " tile " << xt1 << "," << yt1 << std::endl;
			t0 = interpolate_points(x1, y1, x0, y0, xt1, yt1, xt0, yt0, t0);
		}
		else {
			std::cout << "[ERROR] unclear situation at " << x0 << "," << y0 << std::endl;
		}
		x0 = x1;
		y0 = y1;
		xt0 = xt1;
		yt0 = yt1;
		//t0 = t1; removed if favor of additional interpolation
	}
	return;
}


Tile* Loader::interpolate_points(int x1, int y1, int x0, int y0, int xt1, int yt1, int xt0, int yt0, Tile* tlast)
{
	//creates segments in all traversed tiles
	//returns the last tile worked on

	//fill vector with all grid intersections, sort them and insert into tiles 
	std::vector<std::vector<int>> my_vec = { };
	int dxt = get_sign(xt1 - xt0);
	int dyt = get_sign(yt1 - yt0);
	int x = 0, y = 0;
	int xt = 0, yt = 0;
	int xn = 0, yn = 0;
	int l = 0;
	bool found_double = false;
	Tile* t = NULL;

	//add first point to vector
	my_vec.push_back({ x0, y0, 0 });

	//get all grid intersections with x-borders
	for (int i = 0; i < abs(xt1 - xt0); i++)
	{
		xt = (dxt > 0) ? (xt0 + (i + 1) * dxt) : (xt0 + i * dxt); //different when going negative
		x = correct_interpolation(xt * data::s, x0, x1, y0, y1);
		y = (x1 == x0) ? (y0 + (y1 - y0)) : (y0 + ((y1 - y0) * (x - x0)) / (x1 - x0));
		yt = get_tile_index(y);
		l = abs(x - x0) + abs(y - y0);
		if (!found_point(my_vec, x, y, found_double))
		{
			my_vec.push_back({ x, y, l });
		}
	}

	//get all grid intersections with y-borders
	for (int i = 0; i < abs(yt1 - yt0); i++)
	{
		yt = (dyt > 0) ? (yt0 + (i + 1) * dyt) : (yt0 + i * dyt); //different when going negative
		y = correct_interpolation(yt * data::s, y0, y1, x0, x1);
		x = (y1 == y0) ? (x0 + (x1 - x0)) : (x0 + ((x1 - x0) * (y - y0)) / (y1 - y0));
		xt = get_tile_index(x);
		l = abs(x - x0) + abs(y - y0);
		if (!found_point(my_vec, x, y, found_double))
		{
			my_vec.push_back({ x, y, l });
		}
	}

	//add last point to vector
	int lmax = abs(x1 - x0) + abs(y1 - y0);
	my_vec.push_back({ x1, y1, lmax });

	//if (found_double) {
	//	print_interpolation_vector(my_vec);
	//}
	//debugging jump points
	//if (x0 == 0 && y0 == 3400) {
	//	std::cout << "[DEBUG] interpolation " << x1 << "," << y1 << std::endl;
	//	print_interpolation_vector(my_vec);
	//}

	//for all points, find the next point and place both into correct tile
	//first for all but the last, second for all but the first
	for (auto& a : my_vec)
	{
		x = a[0];
		y = a[1];
		l = a[2];
		if (l < lmax)
		{
			std::tie(xn, yn) = find_next_bigger_point(my_vec, l);
			xt = get_tile_index((x + xn) / 2);
			yt = get_tile_index((y + yn) / 2);
			//std::cout << "found " << xn << "," << yn << " tile " << xt << "," << yt << std::endl;
		}
		if (l < lmax && (xn != 0 || yn != 0))
		{
			t = find_tile(xt, yt, true);
			if (t)
			{
				if (t == tlast) {
					t->rails.back().push_back(Point(x, y, 4294967295));
					t->rails.back().push_back(Point(xn, yn, 4294967295));
				}
				else {
					t->add_initial_rail(x, y);
					t->rails.back().push_back(Point(xn, yn, 4294967295));
				}
				//std::cout << "interpolation at " << xn << "," << yn << " tile " << t->xt << "," << t->yt << std::endl;
				data::total_rails++;
			}
			else
			{
				std::cout << "[ERROR] tile not found for " << x << "," << y << std::endl;
			}
		}
	}
	return t;
}


int Loader::correct_interpolation(int x, int x0, int x1, int y0, int y1)
{
	//correct interpolation to keep orientation fixed
	if (y0 != y1)
	{
		int f = abs((x1 - x0) / (y1 - y0));
		while (f >= 2 && (x - x0) % f != 0) { x++; }
	}
	return x;
}


bool Loader::found_point(std::vector<std::vector<int>>& my_vec, int x, int y, bool& found_double)
{
	for (auto& a : my_vec)
	{
		if (a[0] == x && a[1] == y)
		{
			//std::cout << "interpolation: point already here " << x << "," << y << std::endl;
			found_double = true;
			return true;
		}
	}
	return false;
}


std::tuple<int, int> Loader::find_next_bigger_point(std::vector<std::vector<int>>& my_vec, int l)
{
	//find closest next point - point with next bigger length
	int lmin = 1000000, dl = 0;
	int xmin = 0, ymin = 0;
	for (auto& a : my_vec)
	{
		dl = a[2] - l;
		if (dl > 0 && dl < lmin)
		{
			lmin = dl;
			xmin = a[0];
			ymin = a[1];
		}
	}
	if (xmin == 0 && ymin == 0)
	{
		std::cout << "interpolation: no next point for pair found for length: " << l << std::endl;
		print_interpolation_vector(my_vec);
	}
	return std::make_tuple(xmin, ymin);
}


void Loader::print_interpolation_vector(std::vector<std::vector<int>>& my_vec)
{
	std::cout << "vector (" << my_vec.size() << ") ";
	for (auto& a : my_vec) { std::cout << a[0] << "," << a[1] << "," << a[2] << " "; }
	std::cout << std::endl;
	return;
}


void Loader::extract_label(std::string& my_line)
{
	size_t xi = my_line.find("x=\"");
	size_t yi = my_line.find("y=\"");
	size_t li = my_line.find("\">");
	size_t ei = my_line.find("</text>");

	if (xi != std::string::npos && yi != std::string::npos &&
		li != std::string::npos && ei != std::string::npos)
	{
		int xt = std::stoi(my_line.substr(xi + 3, yi - xi - 5));
		int yt = std::stoi(my_line.substr(yi + 3, li - yi - 3));
		std::string label = my_line.substr(li + 2, ei - li - 2);
		Tile* t = find_tile(get_tile_index(xt), get_tile_index(yt), true);
		t->labels.push_back(Label(xt, yt, label));
		data::total_labels++;
		//std::cout << label << std::endl;
	}
	return;
}


void Loader::extract_platf(std::string& my_line)
{
	//extract coords from path string
	std::vector<float> my_vec = {};
	std::string my_num = "";
	std::string my_path = my_line.substr(11, my_line.length() - 15);
	//std::cout << ">>" << my_path << "<<" << std::endl;

	for (char c : my_path)
	{
		if (c != ' ' && c != ',') {
			my_num.push_back(c);
		}
		else if (my_num != "") {
			//std::cout << my_num << std::endl;
			my_vec.push_back(std::stof(my_num));
			my_num = "";
		}
	}
	fill_tiles_platforms(my_vec);
	return;
}


void Loader::fill_tiles_platforms(std::vector<float> my_vec)
{
	int xt = 0;
	int yt = 0;
	Tile* t = NULL;
	std::vector<Pointf> new_vec = {};

	for (unsigned i = 0; i < my_vec.size(); i += 2)
	{
		xt += (int)my_vec[i];
		yt += (int)my_vec[i + 1];
		new_vec.push_back(Pointf(my_vec[i], my_vec[i + 1]));
	}
	if (my_vec.size() > 0)
	{
		xt = 2 * xt / (int)my_vec.size();
		yt = 2 * yt / (int)my_vec.size();
		xt = get_tile_index(xt);
		yt = get_tile_index(yt);
		t = find_tile(xt, yt, true);
	}
	if (t)
	{
		t->platforms.push_back(new_vec);
		data::total_platforms++;
	}
	return;
}


//bool Loader::extract_platforms(std::vector<std::string>& my_raw)
//{
//	//extract coords from file
//	std::vector<float> my_vec = {};
//	std::string my_num = "";
//	bool include = false;
//
//	for (std::string& my_line : my_raw)
//	{
//		if (my_line == "</g>")
//		{
//			include = false;
//		}
//		if (include && my_line.length() > 6 && my_line.substr(0, 5) == "<path")
//		{
//			std::cout << my_line << std::endl;
//			my_line.erase(0, 11);
//			my_line.erase(my_line.length() - 4);
//			my_line.push_back(' ');
//
//			for (char c : my_line)
//			{
//				if (c != ' ' && c != ',') {
//					my_num.push_back(c);
//				}
//				else if (my_num != "") {
//					//std::cout << my_num << std::endl;
//					my_vec.push_back(std::stof(my_num));
//					my_num = "";
//				}
//			}
//			fill_tiles_platforms(my_vec);
//			my_vec.clear();
//		}
//		if (my_line == "<g id=\"stats_platf\">")
//		{
//			include = true;
//		}
//	}
//	return true;
//}


//void Loader::interpolate_points(int x1, int y1, int x0, int y0, int xt1, int yt1, int xt0, int yt0)
//{
//	int x = 0;
//	int y = 0;
//	if (xt1 != xt0)
//	{
//		x = (xt0 > xt1) ? xt0 * data::s : xt1 * data::s;
//		x = correct_interpolation(x, x0, x1, y0, y1);
//		y = y0 + ((y1 - y0) * (x - x0)) / (x1 - x0);
//		//std::cout << "found horizontal interpolate " << x << "," << y << std::endl;
//	}
//	else if (yt1 != yt0)
//	{
//		y = (yt0 > yt1) ? yt0 * data::s : yt1 * data::s;
//		y = correct_interpolation(y, y0, y1, x0, x1);
//		x = x0 + ((x1 - x0) * (y - y0)) / (y1 - y0);
//		//std::cout << "found vertical interpolate " << x << "," << y << std::endl;
//	}
//	//std::cout << "add last point inside" << std::endl;
//	Tile* t0 = find_tile(xt0, yt0, false);
//	if (t0) {
//		t0->rails.back().push_back(Point(x, y, 0));
//		data::total_rails++;
//	}
//	//std::cout << "add next point outside" << std::endl;
//	Tile* t1 = find_tile(xt1, yt1, true);
//	if (t1) {
//		t1->add_initial_rail(x, y);
//		data::total_rails++;
//	}
//	//std::cout << "corrected interpolation " << x << "," << y << std::endl;
//	return;
//}