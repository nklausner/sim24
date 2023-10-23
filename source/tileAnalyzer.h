#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "data.h"


//2023-03-26: initialize tile analyzer
//find irregularites within tiles, inital reason:
//--- segment: -12700, 14700, 1, 0, 21, 424.328
//--- ERROR: next point not found at : -12700, 14700


struct TileAnalyzer
{
	TileAnalyzer();
	void print_current_tile();
};