#pragma once
#include <iostream>
//#include "Tile.h"


//2022-06-20: initialize geometry

namespace geom
{
	extern double xrel[24];
	extern double yrel[24];
}
void create_full_circle();
double get_distance(int dx, int dy, int o);
double get_distance(double dx, double dy, int o);

int get_orientation(int dx, int dy);
//int get_orientation(Point& p0, Point& p1);
bool is_allowed_turn(int o0, int o1);
int get_sign(int x);
double get_line_factor(int x, int y, int x0, int y0, int x1, int y1);

unsigned addcyc(unsigned i, unsigned imax);
unsigned subcyc(unsigned i, unsigned imax);