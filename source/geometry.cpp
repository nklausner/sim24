#include "geometry.h"


void create_full_circle(int(&xa)[24], int(&ya)[24])
{
	int a[12] = { 1000, 972, 894, 707, 447, 243 , 0 , -243, -447, -707, -894 , -972 };
	for (int i = 0; i < 12; i++)
	{
		xa[i] = a[i];
		xa[i + 12] = -a[i];
		ya[i + 6] = a[i];
		ya[(i + 18) % 24] = -a[i];
	}
	return;
}



int find_orientation(int dx, int dy)
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
