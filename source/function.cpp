#include "function.h"


const char * functionName[7] =
{
	"none",
	"stop",
	"switch",
	"signal",
	"timetable",
	"reverse",
	"speed"
};


Function::Function(std::string type_, std::string name_, int x_, int y_, int o0_, int o1_)
{
	type = NONE;
	name = name_;
	x = x_;
	y = y_;
	o0 = o0_;
	o1 = o1_;
	t = 0;
	v = 0;

	//fill vectors with lines for left/right direction
	if (type_ == "switches")
	{
		//starting with o0 left, o1 right
		type = SWITCH;
		std::string mysegment;
		if (name_.size() > 4 && name_.substr(0, 4) == "left")
		{
			std::stringstream mystream(name_.erase(0, 5));
			while (std::getline(mystream, mysegment, ' '))
			{
				linelist.push_back(mysegment);
			}
			//swicth primary and secondary orientation
			o0 = o1_;
			o1 = o0_;
		}
		if (name_.size() > 5 && name_.substr(0, 5) == "right")
		{
			std::stringstream mystream(name_.erase(0, 6));
			while (std::getline(mystream, mysegment, ' '))
			{
				linelist.push_back(mysegment);
			}
		}
		//if (comm_.size() > 6 && comm_.substr(0, 6) == "center")
		//{
		//	std::stringstream mystream(comm_.erase(0, 7));
		//	while (std::getline(mystream, mysegment, ' '))
		//	{
		//		lcenter.push_back(mysegment);
		//	}
		//}
	}
	else if (type_ == "stops")
	{
		type = STOP;
		t = get_stop_duration(name_);
	}
	else if (type_ == "timetables")
	{
		type = TIMETABLE;
		//do nothing, all necessary information are name and orie
	}
	else if (type_ == "reverse")
	{
		type = REVERSE;
		//do nothing, all necessary information are name and orie
	}
	else if (type_ == "speed")
	{
		type = SPEED;
		v = get_speed_value(name_);
	}
	else
	{
		std::cout << "[ERROR] unknown function type " << type_ << std::endl;
	}
	return;
}


int Function::get_type()
{
	return type;
}
std::string Function::get_name()
{
	return name;
}
int Function::get_orientation()
{
	return o0;
}
int Function::get_time()
{
	return t;
}
int Function::get_speed()
{
	return v;
}


int Function::decide_orientation(std::string& myline, std::string& mylinedest, int myorie)
{
	//abort no orientation defined or opposite orientation
	//print_switch();
	//std::cout << "switch (" << x << "," << y << ") " << myline << " ";
	if (std::abs(myorie - o0) > 1 || o0 == -1 || o1 == -1)
	{
		//std::cout << "continue" << std::endl;
		return -1;
	}
	else if (std::find(linelist.begin(), linelist.end(), myline) != linelist.end())
	{
		//std::cout << "branch off " << o1 << std::endl;
		return o1;
	}
	else if (std::find(linelist.begin(), linelist.end(), mylinedest) != linelist.end())
	{
		//std::cout << "branch off " << o1 << std::endl;
		return o1;
	}
	//else if (std::find(lleft.begin(), lleft.end(), myline) != lleft.end())
	//{
	//	std::cout << "switch: "<< myline <<" turns left" << std::endl;
	//	return oleft;
	//}
	//else if (linelist.empty())
	//{
	//	std::cout << "switch: go right" << std::endl;
	//	return oright;
	//}
	//else if (lleft.empty())
	//{
	//	std::cout << "switch: go left" << std::endl;
	//	return oleft;
	//}
	//std::cout << "no case applied" << std::endl;
	return o0;
}


void Function::print_switch()
{
	std::cout << "switch " << x << "," << y;
	std::cout << " branch(" << o1 << "):";
	for (auto s : linelist) { std::cout << " " << s; }
	//std::cout << " right(" << oright << "):";
	//for (auto s : lright) { std::cout << " " << s; }
	//std::cout << " center:";
	//for (auto s : lcenter) { std::cout << " " << s; }
	std::cout << std::endl;
	return;
}


int Function::get_stop_duration(std::string& mystr)
{
	//std::cout << "stop " << mystr << std::endl;
	if (mystr.length() > 4)
	{
		size_t i0 = mystr.find("time") + 4;
		size_t i1 = mystr.find(" ", i0);
		mystr = mystr.substr(i0, i1 - i0);
		//std::cout << "stop-time: " << mystr << std::endl;
		return (std::stoi(mystr) + 15);
	}
	return 45;
}


int Function::get_speed_value(std::string& mystr)
{
	//std::cout << "stop " << mystr << std::endl;
	if (mystr.length() > 5)
	{
		mystr = mystr.substr(6);
		//std::cout << "speed limit >>" << mystr << "<<" << std::endl;
		return (std::stoi(mystr));
	}
	return 80;
}