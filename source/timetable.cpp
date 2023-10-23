#include "timetable.h"


Timetable::Timetable(std::string& name_, int& x_, int& y_, int& o_)
{
	name = name_;
	x = x_;
	y = y_;
	o = o_;
	//std::cout << "created timetable: " << name << " " << x << " " << y << std::endl;
	return;
}


void Timetable::complete_timetable(std::string& line_, std::vector<std::string>& my_raw)
{
	line = line_;
	table = write_table(my_raw);
	//std::cout << "completed timetable: " << name << " " << line << std::endl;
	return;
}


std::string Timetable::write_table(std::vector<std::string>& my_raw)
{
	std::string tt = "";
	size_t i = 0;
	for (std::string &my_line : my_raw)
	{
		for (char c : my_line)
		{
			if (c != ' ')
			{
				tt.push_back(c);
				i++;
			}
			else
			{
				while (i < 3) {
					tt.push_back(' ');
					i++;
				}
				tt.push_back(',');
				i = 0;
			}
		}
		while (i < 3) {
			tt.push_back(' ');
			i++;
		}
		tt.push_back(';');
		i = 0;
	}
	//std::cout << tt << std::endl;
	return tt;
}


std::string Timetable::get_name()
{
	return name;
}
int Timetable::getx()
{
	return x;
}
int Timetable::gety()
{
	return y;
}
int Timetable::geto()
{
	return o;
}


int Timetable::get_departure(int &tnow)
{
	//returns next available unreserved departure in ms
	int tnext = -1;
	while (tnext <= tlast || tnext < tnow)
	{
		if (table[index + 3] == ';')
		{
			ihour++;
		}
		index += 4;
		//reset at 03:00 (27:00)
		if (ihour == 27 || index == table.length()) {
			index = 0;
			ihour = 3;
			tlast = 0;
		}
		if (table[index] != '-')
		{
			imin = std::stoi(table.substr(index, 2));
			tnext = 3600000 * ihour + 60000 * imin;
		}
	}
	//std::cout << "timetable " << line << " " << name << " at " << ihour << ":" << imin << std::endl;
	//std::cout << tnext << std::endl;
	//tlast = tnext;
	return tnext;
}
std::string Timetable::get_destination_line(std::string myline)
{
	//returns string of line + destination of the current next departure
	//default just return line as given
	if (table[index + 2] != ' ')
	{
		//std::cout << "linedest: " << myline << "_" << table[index + 2] << std::endl;
		return (myline + "_" + table[index + 2]);
	}
	return myline;
}