#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "data.h"


//2023-06-05: start with outsourcing loading


std::vector<std::string> read_file(std::string &filename);
std::string extract_attribute(std::string my_key, std::string &my_line);
std::vector<std::string> extract_children(std::vector<std::string> &my_raw);
std::string trim_string(std::string my_str);