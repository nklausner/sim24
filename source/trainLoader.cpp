#include "trainLoader.h"


void extract_timetables(std::vector<std::string>& my_raw, std::vector<Timetable>& my_timetables)
{
	//update timetable object with actual tables and line
	std::string line_name = "0000";
	std::vector<std::string> my_vec = {};
	std::string my_name = "";
	bool include = false;

	for (auto &my_line : my_raw)
	{
		if (my_line.length() > 10 && my_line.substr(0, 10) == "<line name")
		{
			line_name = extract_attribute("name", my_line);
		}
		if (include && my_line == "</timetable>")
		{
			//at the end update matching timetable object
			for (auto& mytt : my_timetables)
			{
				if (mytt.get_name() == my_name)
				{
					mytt.complete_timetable(line_name, my_vec);
					include = false;
					break;
				}
			}
			if (include)
			{
				std::cout << "[ERROR] no timetable function for: " << my_name << std::endl;
			}
			my_vec.clear();
			include = false;
		}
		if (include)
		{
			my_vec.push_back(my_line);
		}
		if (!include && my_line.length() > 10 && my_line.substr(0, 10) == "<timetable")
		{
			my_name = extract_attribute("name", my_line);
			include = true;
		}
	}
	return;
}


void extract_trains(std::vector<std::string>& my_raw, std::vector<Train>& my_trains)
{
	my_trains.clear();
	std::string line_name = "0000";

	for (auto &my_line : my_raw)
	{
		if (my_line.length() > 10 && my_line.substr(0, 10) == "<line name")
		{
			line_name = extract_attribute("name", my_line);
		}
		if (my_line.length() > 7 && my_line.substr(0, 6) == "<train")
		{
			std::string my_role = extract_attribute("role", my_line);
			std::string my_start = extract_attribute("start", my_line);
			std::string my_comp = extract_attribute("comp", my_line);
			std::string my_test = extract_attribute("test", my_line);
			//std::cout << my_role << " " << my_start << " " << my_comp << " " << my_test << std::endl;

			for (auto& mytt : data::timetables)
			{
				if (mytt.get_name() == my_start)
				{
					my_trains.push_back(Train(line_name, my_role, my_comp, my_start, mytt.getx(), mytt.gety(), mytt.geto()));
					break;
				}
			}
		}
	}
	return;
}


void position_train()
{
	return;
}


void fill_series_length()
{
	data::series_length["0423.0"] = 18.2f;
	data::series_length["0433.0"] = 15.4f;
	data::series_length["0433.5"] = 15.4f;
	data::series_length["0423.5"] = 18.2f;
	data::series_length["0430.0"] = 19.2f;
	data::series_length["0431.0"] = 15.0f;
	data::series_length["0431.5"] = 15.0f;
	data::series_length["0430.5"] = 19.2f;
	return;
}