#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "function.h"
#include "data.h"
#include "geometry.h"
#include "fileHandler.h"


//2022-12-26: initialize function builder
//take raw function info, place it into tile, find adjacent points
//2023-07-02: changing to function loading, now switches and timetables


struct FunctionLoader
{
public:
	FunctionLoader(); //std::vector<std::string>& my_raw)

private:
	bool extract_function(std::vector<std::string>& my_raw, std::string my_type);
	//void extract_timetables(std::vector<std::string>& my_raw);
	//std::string write_timetable(std::vector<std::string>& my_raw);
	bool place_function(std::vector<int>& v, std::string& comm, std::string& type);
	bool place_timetable(std::string& table, std::string& name, std::string& line);
};