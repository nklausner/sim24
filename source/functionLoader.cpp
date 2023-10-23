#include "functionLoader.h"


FunctionLoader::FunctionLoader()
{
	std::string my_filename = "";
	std::vector<std::string> my_raw = {};
	std::vector<std::string> my_groups = {"switches", "timetables", "functions", "stops"};

	for (std::string gn : my_groups)
	{
		my_filename = data::my_directory + "infrastructure\\" + gn + ".svg";
		my_raw = read_file(my_filename);
		extract_function(my_raw, gn);
	}
	//print debug info
	if (data::is_info)
	{
		std::cout << "[INFO] functions: " << data::total_functions << std::endl;
	}
	return;
}


bool FunctionLoader::extract_function(std::vector<std::string>& my_raw, std::string main_type)
{
	bool include = false;
	std::string my_startline = "<g id=\"" + main_type + "\">";
	std::string my_num = "";
	std::string my_path = "";
	std::string my_comm = "";
	std::string my_type = "";
	std::vector<int> my_vec = {};
	size_t total_functions = 0;

	for (std::string& my_line : my_raw)
	{
		if (my_line == my_startline)
		{
			include = true;
		}
		if (my_line == "</g>")
		{
			include = false;
		}
		if (include && my_line.length() > 6 && my_line.substr(0, 5) == "<path")
		{
			my_type = main_type;
			my_path = extract_attribute("d", my_line);
			my_comm = extract_attribute("comm", my_line);
			//std::cout << "= " << my_path << std::endl;
			//std::cout << "= " << my_comm << std::endl;
			if (main_type == "functions")
			{
				if (my_comm.length() >= 7 && my_comm.substr(0, 7) == "reverse")
				{
					my_type = "reverse";
				}
				else if (my_comm.length() >= 5 && my_comm.substr(0, 5) == "speed")
				{
					my_type = "speed";
				}
				//std::cout << my_comm << std::endl;
			}
			if (main_type == "stops")
			{
				if (my_comm == "")
				{
					my_comm = "stop";
				}
			}

			if (my_path != "" && my_comm != "")
			{
				my_vec.clear();
				for (char c : my_path)
				{
					if (c != ' ' && c != ',' && c != 'M')
					{
						my_num.push_back(c);
					}
					else if (my_num != "")
					{
						my_vec.push_back(std::stoi(my_num));
						my_num = "";
					}
				}
				if (my_vec.size() >= 4)
				{
					place_function(my_vec, my_comm, my_type);
					total_functions++;
				}
			}
		}
	}
	//std::cout << "[DEBUG] to place: " << total_functions << " " << my_type << std::endl;
	return true;
}


bool FunctionLoader::place_function(std::vector<int>& v, std::string& comm, std::string& type)
{
	//set charecterics for one piece (one orientation) function
	int x = v[0];
	int y = v[1];
	int o0 = get_orientation(v[2] - v[0], v[3] - v[1]);
	int o1 = o0;
	bool is_placed = false;

	if (type == "switches" && v.size() == 6)
	{
		//set characteristics of single switch and place it
		x = v[2];
		y = v[3];
		o0 = get_orientation(v[0] - v[2], v[1] - v[3]);
		o1 = get_orientation(v[4] - v[2], v[5] - v[3]);

		if (o1 == (o0 + 1) % 24) //o1 right
		{
			//do nothing
		}
		else if (o0 == (o1 + 1) % 24) //o0 right
		{
			//switch so o1 is right
			int o2 = o0;
			o0 = o1;
			o1 = o2;
		}
		else
		{
			std::cout << "[ERROR] switch orientation incorrect " << x << "," << y << std::endl;
		}
	}

	Tile* t = find_tile(get_tile_index(x), get_tile_index(y), false);
	if (t)
	{
		//create the function object as next newest entry
		size_t my_index = t->functions.size();
		t->functions.push_back(Function(type, comm, x, y, o0, o1));

		//if (v[2] == -10110 && v[3] == 13360)
		//{
		//	std::cout << "test switch " << v[2] << "," << v[3] << " " << my_index << std::endl;
		//}

		for (size_t j = 0; j < t->rails.size(); j++)
		{
			for (size_t i = 1; i < t->rails[j].size(); i++)
			{
				double f = get_line_factor(x, y, t->rails[j][i - 1].x, t->rails[j][i - 1].y, t->rails[j][i].x, t->rails[j][i].y);

				if (f >= 0.0 && f <= 1.0) //p on line p0 -> p1
				{
					if (f == 0.0) //p == p0
					{
						t->rails[j][i - 1].findex = my_index;
					}
					else if (f == 1.0) //p == p1
					{
						t->rails[j][i].findex = my_index;
					}
					else //p inbetween
					{
						std::vector<Point>::iterator iinsert = t->rails[j].begin() + i;
						t->rails[j].insert(iinsert, Point(x, y, my_index));
					}
					i = t->rails[j].size(); //check next rails
					data::total_functions++;
					is_placed = true;
					//if (v[2] == -10110 && v[3] == 13360)
					//{
					//	std::cout << "factor: " << f << " functions: " << t->functions.size() << std::endl;
					//}
				}
			}
		}
		//create empty timetables with just name and position
		if (type == "timetables" and is_placed)
		{
			data::timetables.push_back(Timetable(comm, x, y, o0));
		}
		if (x == 20276 && y == 10994)
		{
			//std::cout << "test" << std::endl;
		}
	}
	else
	{
		std::cout << "[ERROR] tile not found for function " << x << "," << y << std::endl;
		return false;
	}
	if (data::is_debug && !is_placed)
	{
		std::cout << "[DEBUG] did not place function " << x << "," << y << std::endl;
	}
	return true;
}


bool FunctionLoader::place_timetable(std::string& table, std::string& name, std::string& line)
{
	return true;
}