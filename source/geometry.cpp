#include "geometry.h"


namespace geom
{
	double xrel[24] = { 0.0 };
	double yrel[24] = { 0.0 };
}


void create_full_circle()
{
	double a[12] = { 1.0, 0.972, 0.894, 0.707, 0.447, 0.243 , 0.0 , -0.243, -0.447, -0.707, -0.894 , -0.972 };
	for (int i = 0; i < 12; i++)
	{
		geom::xrel[i] = a[i];
		geom::xrel[i + 12] = -a[i];
		geom::yrel[i + 6] = a[i];
		geom::yrel[(i + 18) % 24] = -a[i];
	}
	return;
}


double get_distance(int dx, int dy, int o)
{
	return get_distance((double)dx, (double)dy, o);
}
double get_distance(double dx, double dy, int o)
{
	if (geom::xrel[o] != 0.0) { return dx / geom::xrel[o]; }
	if (geom::yrel[o] != 0.0) { return dy / geom::yrel[o]; }
	return 0.0;
}



int get_orientation(int dx, int dy)
{
	if (dy == 0) {
		return (dx > 0) ? 0 : 12;
	}
	if (dx == 0) {
		return (dy > 0) ? 6 : 18;
	}
	if (dx == dy) {
		return (dy > 0) ? 3 : 15;
	}
	if (dx == -dy) {
		return (dy > 0) ? 9 : 21;
	}
	if (dx == 2 * dy) {
		return (dy > 0) ? 2 : 14;
	}
	if (dx == -2 * dy) {
		return (dy > 0) ? 10 : 22;
	}
	if (2 * dx == dy) {
		return (dy > 0) ? 4 : 16;
	}
	if (2 * dx == -dy) {
		return (dy > 0) ? 8 : 20;
	}
	if (dx == 4 * dy) {
		return (dy > 0) ? 1 : 13;
	}
	if (dx == -4 * dy) {
		return (dy > 0) ? 11 : 23;
	}
	if (4 * dx == dy) {
		return (dy > 0) ? 5 : 17;
	}
	if (4 * dx == -dy) {
		return (dy > 0) ? 7 : 19;
	}
	std::cout << "orientation off " << dx << " " << dy <<  std::endl;
	return -1;
}


//int get_orientation(Point& p0, Point& p1)
//{
//	return get_orientation(p1.x - p0.x, p1.y - p0.y);
//}


bool is_allowed_turn(int o0, int o1)
{
	if (o1 == o0 ||
		o1 == (o0 + 1) % 24 ||
		o0 == (o1 + 1) % 24)
	{
		return true;
	}
	return false;
}


int get_sign(int x)
{
	return (x > 0) ? +1 : ((x < 0) ? -1 : 0);
}


double get_line_factor(int x, int y, int x0, int y0, int x1, int y1)
{
	//if (x == -10110 && y == 13360)
	//{
	//	std::cout << "debug here " << x0 << "," << y0 << std::endl;
	//}

	//returns factor for calculating point position (p = p0 + f * (p1 - p0))
	double f = -1.0;
	double dxp = x - x0;
	double dyp = y - y0;
	double dxl = x1 - x0;
	double dyl = y1 - y0;
	if (dxl != 0)
	{
		double f = dxp / dxl;
		//if (x0 == -10110 && y0 == 13360)
		//{
		//	std::cout << "debug here dxf:";
		//	if (f >= 0) { std::cout << " f >= 0,"; }
		//	if (f <= 1) { std::cout << " f <= 1,"; }
		//	if (f*dyl == dyp) { std::cout << " f*dyl==dyp,"; }
		//	std::cout << f * dyl - dyp;
		//	std::cout << std::endl;
		//}
		if (f >= 0 && f <= 1 &&	std::abs(f * dyl - dyp) < 0.001)
		{
			//std::cout << "on line " << x0 << "," << y0 << std::endl;
			return f;
		}
	}
	if (dyl != 0)
	{
		double f = dyp / dyl;
		//if (x0 == -10110 && y0 == 13360)
		//{
		//	std::cout << "debug here dxf:";
		//	if (f >= 0) { std::cout << " f >= 0,"; }
		//	if (f <= 1) { std::cout << " f <= 1,"; }
		//	if (f*dxl == dxp) { std::cout << " f*dxl==dxp,"; }
		//	std::cout << f * dxl - dxp;
		//	std::cout << std::endl;
		//}
		if (f >= 0 && f <= 1 && std::abs(f * dxl - dxp) < 0.001)
		{
			//std::cout << "on line " << x0 << "," << y0 << std::endl;
			return f;
		}
	}
	return f;
}


unsigned addcyc(unsigned i, unsigned imax)
{
	return (i == imax - 1) ? 0 : (i + 1);
}
unsigned subcyc(unsigned i, unsigned imax)
{
	return (i == 0) ? (imax - 1) : (i - 1);
}