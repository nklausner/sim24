#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>


//2022-12-26: initialize function class for stops, switches, signals
//holding the information what to do at a switch point
//type: 0 = none, 1 = stop, 2 = switch, 3 = signal ... (+ many more to come)


class Function
{
public:
	Function(std::string type_, std::string name_, int x_, int y_, int o0_, int o1_);

	int get_type();
	std::string get_name();
	int get_orientation();
	int get_time();
	int get_speed();
	int decide_orientation(std::string& myline, std::string& mylinedest, int myorie);
	void print_switch();

private:
	//general properties
	int type = 0;
	std::string name = "";
	int x = 0;
	int y = 0;
	int o0 = -1;
	int o1 = -1;
	int t = 0;
	int v = 0;
	std::vector<std::string> linelist = {};

	//methods
	int get_stop_duration(std::string& mystr);
	int get_speed_value(std::string& mystr);
};


enum FunctionTypes
{
	NONE,
	STOP,
	SWITCH,
	SIGNAL,
	TIMETABLE,
	REVERSE,
	SPEED
};


extern const char * functionName[7];