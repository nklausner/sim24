#include "loader.h"


bool Loader::load_all()
{
	std::string my_filename = "";
	for (auto& s : data::infra_files)
	{
		my_filename = data::my_directory + "infrastructure\\" + s;
		read_file(my_filename);
	}
	std::cout << "tiles: " << data::tiles.size() << std::endl;
	std::cout << "rails: " << data::total_rails << std::endl;
	std::cout << "platforms: " << data::total_platforms << std::endl;
	std::cout << "labels: " << data::total_labels << std::endl;
	return true;
}


bool Loader::read_file(std::string &filename)
{
	//open file and read lines into vector
	std::vector<std::string> my_vec = {};
	std::string my_line;
	std::ifstream my_file(filename);

	if (my_file.is_open())
	{
		while (std::getline(my_file, my_line))
		{
			my_vec.push_back(my_line);
		}
		my_file.close();
		extract_rails(my_vec);
		extract_platforms(my_vec);
		extract_infra(my_vec);
	}
	else
	{
		std::cout << "Error reading: " << filename << std::endl;
		return false;
	}
	return true;
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
			if (my_line == "<g id=\"stats_label\">") { my_modus = 3; }
		}
		else 
		{
			if (my_line == "</g>") { my_modus = 0; continue; }
			switch (my_modus)
			{
			case 1: break;
			case 2: break;
			case 3: extract_label(my_line);  break;
			}
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
		if (include)
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
	int x1, y1 = 0;
	int x0, y0 = 0;
	int xt1, yt1 = 0;
	int xt0, yt0 = 0;
	Tile* t1 = NULL;
	Tile* t0 = NULL;
	for (unsigned i = 0; i < my_vec.size(); i += 2)
	{
		x1 = my_vec[i];
		y1 = my_vec[i+1];
		xt1 = get_tile_index(x1);
		yt1 = get_tile_index(y1);

		if (i == 0)
		{
			//std::cout << "add first point" << std::endl;
			t1 = find_tile(xt1, yt1, true);
			t1->add_initial_rail(x1, y1);
			data::total_rails++;
		}
		else if (t1 && t1->xt == xt1 && t1->yt == yt1)
		{
			//std::cout << "add next point inside" << std::endl;
			t1->rails.back().push_back(Point(x1, y1, 0));
			data::total_rails++;
		}
		else
		{
			if (xt1 != xt0 || yt1 != yt0)
			{
				interpolate_points(x1, y1, x0, y0, xt1, yt1, xt0, yt0);
			}
			//std::cout << "add next point inside" << std::endl;
			t1 = find_tile(xt1, yt1, false);
			t1->rails.back().push_back(Point(x1, y1, 0));
			data::total_rails++;
		}
		x0 = x1;
		y0 = y1;
		xt0 = xt1;
		yt0 = yt1;
		t0 = t1;
	}
	return;
}


void Loader::interpolate_points(int x1, int y1, int x0, int y0, int xt1, int yt1, int xt0, int yt0)
{
	is_corrected = false;
	int x = 0;
	int y = 0;
	if (xt1 != xt0)
	{
		x = (xt0 > xt1) ? xt0 * data::s : xt1 * data::s;
		x = correct_interpolation(x, x0, x1, y0, y1);
		y = y0 + ((y1 - y0) * (x - x0)) / (x1 - x0);
		//std::cout << "found horizontal interpolate " << x << "," << y << std::endl;
	}
	else if (yt1 != yt0)
	{
		y = (yt0 > yt1) ? yt0 * data::s : yt1 * data::s;
		y = correct_interpolation(y, y0, y1, x0, x1);
		x = x0 + ((x1 - x0) * (y - y0)) / (y1 - y0);
		//std::cout << "found vertical interpolate " << x << "," << y << std::endl;
	}
	//std::cout << "add last point inside" << std::endl;
	Tile* t0 = find_tile(xt0, yt0, false);
	if (t0) {
		t0->rails.back().push_back(Point(x, y, 0));
		data::total_rails++;
	}
	//std::cout << "add next point outside" << std::endl;
	Tile* t1 = find_tile(xt1, yt1, true);
	if (t1) {
		t1->add_initial_rail(x, y);
		data::total_rails++;
	}
	//if (is_corrected)
	//{
	//	std::cout << "corrected interpolation " << x << "," << y << std::endl;
	//}
	return;
}


int Loader::correct_interpolation(int x, int x0, int x1, int y0, int y1)
{
	//correct interpolation to keep orientation fixed
	if (y0 != y1)
	{
		int f = abs((x1 - x0) / (y1 - y0));
		while (f >= 2 && (x - x0) % f != 0) {
			x++;
			is_corrected = true;
		}
	}
	return x;
}


bool Loader::extract_platforms(std::vector<std::string>& my_raw)
{
	//extract coords from file
	std::vector<float> my_vec = {};
	std::string my_num = "";
	bool include = false;

	for (std::string& my_line : my_raw)
	{
		if (my_line == "</g>")
		{
			include = false;
		}
		if (include)
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
					my_vec.push_back(std::stof(my_num));
					my_num = "";
				}
			}
			fill_tiles_platforms(my_vec);
			my_vec.clear();
		}
		if (my_line == "<g id=\"stats_platf\">")
		{
			include = true;
		}
	}
	return true;
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