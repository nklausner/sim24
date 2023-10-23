#include "fileHandler.h"


std::vector<std::string> read_file(std::string &filename)
{
	//open file and read lines into vector
	std::vector<std::string> my_vec = {};
	std::string my_line;
	std::ifstream my_file(filename);

	if (my_file.is_open())
	{
		if (data::is_info)
		{
			std::cout << "[INFO] reading: " << filename << std::endl;
		}
		while (std::getline(my_file, my_line))
		{
			my_line = trim_string(my_line);
			my_vec.push_back(my_line);
		}
		my_file.close();
		//std::cout << "trimmed chars: " << data::testcount << std::endl;
	}
	else
	{
		std::cout << "[ERROR] reading: " << filename << std::endl;
	}
	return my_vec;
}



std::string extract_attribute(std::string my_key, std::string &my_line)
{
	my_key = ' ' + my_key + '=' + '\"';
	size_t i0 = my_line.find(my_key);
	size_t i1 = std::string::npos;
	if (i0 != std::string::npos)
	{
		i0 += my_key.length();
		i1 = my_line.find('\"', i0);
	}
	if (i1 != std::string::npos)
	{
		return my_line.substr(i0, i1 - i0);
	}
	return "";
}


std::vector<std::string> extract_children(std::vector<std::string> &my_raw)
{
	return my_raw;
}


std::string trim_string(std::string my_str)
{
	if (!my_str.empty())
	{
		while (my_str.front() == ' ' || my_str.front() == '\t')
		{
			my_str.erase(0, 1);
			data::testcount++;
		}
	}
	if (!my_str.empty())
	{
		while (my_str.back() == ' ' || my_str.back() == '\t')
		{
			my_str.pop_back();
			data::testcount++;
		}
	}
	return my_str;
}