#pragma once
#include "data.h"
#include "geometry.h"


//2022-06-19: initialize train


class Train
{
public:
	Train(std::string line_, std::string role_, std::string comp_, std::string start_, int x0, int y0, int o0);
	void update();
	bool is_visible();
	void set_train_car_coords(std::vector<float>& xvec, std::vector<float>& yvec, std::vector<int>& ovec, std::vector<float>& lvec);

	std::string line = "0000";
	std::string role = "none";
	std::string comp = "0000";
	std::string orig = "";
	std::string dest = "";
	std::string linedest = "0000";

	int status = 0;

	//Location properties
	double x = 0.0;		//current location x_coordinate in meters
	double y = 0.0;		//current location y_coordinate in meters
	int o = 0;			//current orientation of line segment
	double s = 0.0;		//current distance left on line segment in meters

	//update function debugging
	double supdate = 0.0;	//update distance in meters
	int iupdate = 0;		//update call count

	//float cx[32] = { 0.0 };		//xcoords of wagons
	//float cy[32] = { 0.0 };		//ycoords of wagons
	//float larr[32] = { 0.0 };		//lengths of wagons
	//float karr[32] = { 0.0 };		//spacing of wagons
	//int qarr[32] = { 0 };			//orientation of wagons
	float larr[32] = { 0.0 };		//lengths of wagons
	float darr[32] = { 0.0 };		//lengths of wagons (distance from front)
	float length = 0.0;				//train length
	unsigned count = 1;				//count of wagons

private:
	//point arrays (train can stretch over 31 segments)
	const static unsigned maxpoints = 32;
	int an = 1;				//index of next point
	int al = 0;				//index of last point
	int xarr[maxpoints] = { 0 };	//array of xcoords of points
	int yarr[maxpoints] = { 0 };	//array of ycoords of points
	int oarr[maxpoints] = { 0 };	//array of orientations between points
	double sarr[maxpoints] = { 0.0 }; //array of distances between points

	//Tile properties
	int xt = 0;			//xcoord tile
	int yt = 0;			//ycoord tile
	Tile* tile = NULL;	//current tile

	//Path properties
	int ivec = 0;
	int inext = 0;
	int ilast = 0;
	int oswitch = -1;

	//Time properties
	int t = 0;		//current time in milliseconds
	int tstart = 0;	//starting time in milliseconds
	int tstop = 0;	//typical stop time in seconds
	int ttlast = 0; //timetable last time in milliseconds

	//Velocity properties
	double v = 0.0;		//current velocity of train in meters/second
	double vtar = 0.0;	//target velocity of train in meters/second
	double vall = 0.0;	//allowed velocity of line in meters/second
	double vmax = 0.0;	//maximal velocity of train in meters/second
	double acc = 0.0;	//acceleration in meters/squaresecond
	double dec = 0.0;	//deceleration in meters/squaresecond (absolute value)

	void find_next_point();
	Tile* find_next_tile();
	int find_departure(std::string& my_name);
	void set_wagon_spacing(std::string& my_comp);
	void reverse_front();
	void find_current_segment();
	void stop_for(int my_time);
	void stop_till(int my_time);
};


enum States
{
	STOPPED,
	COASTING,
	ACCELERATING,
	DECELERATING,
	BLOCKED
};