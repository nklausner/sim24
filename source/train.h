#pragma once
#include "data.h"
#include "geometry.h"


//2022-06-19: initialize train


class Train
{
public:
	Train(float x, float y, int xl, int yl, int xn, int yn);
	void update();
	void update_cars();

	float cx[32] = { 0.0 };		//xcoords of wagons
	float cy[32] = { 0.0 };		//ycoords of wagons
	float larr[32] = { 0.0 };	//lengths of wagons
	float karr[32] = { 0.0 };	//spacing of wagons
	int qarr[32] = { 0 };		//orientation of wagons
	int length = 1;				//count of wagons

private:
	int an = 1;				//index of next point
	int al = 0;				//index of last point

	const static unsigned maxpoints = 12;
	int xarr[maxpoints] = { 0 };	//array of xcoords of points
	int yarr[maxpoints] = { 0 };	//array of ycoords of points
	float sarr[maxpoints] = { 0.0 }; //array of distances between points
	int oarr[maxpoints] = { 0 };	//array of orientations between points

	int xt = 0;			//xcoord tile
	int yt = 0;			//ycoord tile
	Tile* tile = NULL;	//current tile

	int ivec = 0;
	int inext = 0;
	int ilast = 0;

	float v = 0.0;	//velocity
	int tlast = 0;	//last timestamp

	void find_next_point();
	Tile* find_next_tile();
	Tile* check_tile_for(int tx, int ty, int xs, int ys);
	float get_distance(int dx, int dy, int o);
};