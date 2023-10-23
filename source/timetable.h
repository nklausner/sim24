#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>


//2023-06-25: timetable with string as container


class Timetable
{
public:
	Timetable(std::string& name_, int& x_, int& y_, int& o_);
	void complete_timetable(std::string& line_, std::vector<std::string>& my_raw);
	std::string get_name();
	int getx();
	int gety();
	int geto();
	int get_departure(int &tnow);
	std::string get_destination_line(std::string myline);

private:
	std::string name = "my_timetable_0000";
	std::string line = "0000";
	std::string table = "00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;00;";
	int tlast = -1;
	int imin = 0;
	int ihour = 3;
	unsigned index = 0;
	int x = 0;
	int y = 0;
	int o = -1;

	std::string write_table(std::vector<std::string>& my_raw);
};