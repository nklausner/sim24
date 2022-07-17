#include "train.h"


Train::Train(float x_, float y_, int xl_, int yl_, int xn_, int yn_)
{
	cx[0] = x_;
	cy[0] = y_;
	al = 0;
	an = 1;
	xarr[an] = xn_;
	yarr[an] = yn_;
	xarr[al] = xl_;
	yarr[al] = yl_;
	xt = get_tile_index((int)cx[0]);
	yt = get_tile_index((int)cy[0]);

	tile = check_tile_for(xt, yt, xarr[an], yarr[an]);
	v = (float)40.0 / (float)3.6;
	oarr[al] = find_orientation(xarr[an] - xarr[al], yarr[an] - yarr[al]);
	qarr[0] = oarr[al];
	sarr[al] = get_distance(xarr[an] - xarr[al], yarr[an] - yarr[al], oarr[al]);
	sarr[an] = get_distance(xarr[an] - (int)cx[0], yarr[an] - (int)cy[0], oarr[al]);
	tlast = data::time;

	length = 4;
	for (int i = 0; i < length; i++)
	{
		larr[i] = 25.0;
		if (i > 0) { karr[i] = larr[i-1]/2 + larr[i]/2; }
		qarr[i] = qarr[0];
	}
	std::cout << "train initiated" << std::endl;
	return;
}


void Train::update()
{
	float ds = v / data::fps;
	if (sarr[an] >= ds)
	{
		cx[0] += ds * data::xrel[oarr[al]] / 1000;
		cy[0] += ds * data::yrel[oarr[al]] / 1000;
		sarr[an] -= ds;
	}
	else
	{
		ds -= sarr[an];
		find_next_point();
		if (tile)
		{
			oarr[al] = find_orientation(xarr[an] - xarr[al], yarr[an] - yarr[al]);
			sarr[al] = get_distance(xarr[an] - xarr[al], yarr[an] - yarr[al], oarr[al]);
			cx[0] = xarr[al] + ds * data::xrel[oarr[al]] / 1000;
			cy[0] = yarr[al] + ds * data::yrel[oarr[al]] / 1000;
			qarr[0] = oarr[al];
			sarr[an] = sarr[al] - ds;
			//std::cout << "new segment: " << o << " " << s << std::endl;
			//std::cout << ilast << " " << inext << std::endl;
		}
	}
	return;
}


void Train::find_next_point()
{
	an = (an + 1) % maxpoints;
	al = (al + 1) % maxpoints;

	if (inext > ilast && inext < tile->rails[ivec].size() - 1)
	{
		inext++;
		ilast++;
	}
	else if (inext < ilast && inext > 0)
	{
		inext--;
		ilast--;
	}
	else
	{
		//std::cout << " case: need to change tile" << std::endl;
		tile = find_next_tile();
		if (!tile) {
			std::cout << "next point not found" << std::endl;
			return;
		}
	}
	xarr[an] = tile->rails[ivec][inext].x;
	yarr[an] = tile->rails[ivec][inext].y;
	//std::cout << "-> " << ivec << " " << inext << " " << tile << std::endl;				
	return;
}


Tile* Train::find_next_tile()
{
	Tile* tnew = NULL;
	int xta[8] = { +1, -1, 0, 0, +1, -1, -1, +1 };
	int yta[8] = { 0, 0, +1, -1, +1, -1, +1, -1 };

	for (int i = 0; i < 8; i++)
	{
		tnew = check_tile_for(xt + xta[i], yt + yta[i], xarr[al], yarr[al]);
		if (tnew) {
			xt = tnew->xt;
			yt = tnew->yt;
			return tnew;
		}
	}
	return tnew;
}


Tile* Train::check_tile_for(int tx, int ty, int xs, int ys)
{
	//std::cout << "search for tile (" << tx << "," << ty << ")" << std::endl;
	Tile* ts = find_tile(tx, ty, false);
	if (ts)
	{
		for (int j = 0; j < ts->rails.size(); j++)
		{
			for (int i = 0; i < ts->rails[j].size(); i++)
			{
				if (ts->rails[j][i].x == xs &&
					ts->rails[j][i].y == ys)
				{
					ivec = j;
					if (i == 0)
					{
						ilast = 0;
						inext = 1;
					}
					else if (i == ts->rails[j].size() - 1)
					{
						ilast = i;
						ilast = i - 1;
					}
					else
					{
						ilast = i - 1;
						inext = i;
					}
					//std::cout << j << " " << ilast << " " << inext << " " << ts << std::endl;
					return ts;
				}
			}
		}
	}
	return NULL;
}


float Train::get_distance(int dx, int dy, int o)
{
	if (data::xrel[o] != 0) { return 1000 * (float)dx / data::xrel[o]; }
	if (data::yrel[o] != 0) { return 1000 * (float)dy / data::yrel[o]; }
	return 0.0;
}


void Train::update_cars()
{
	//set the position of all train cars using transition points
	//TODO: position with limited stretch
	int j = maxpoints;
	int a = al;
	float s = sarr[a] - sarr[an];
	for (int i = 1; i < length; i++)
	{
		s -= karr[i];
		while (s < 0)
		{
			if (j > 2)
			{
				j--;
				a = (al + j) % maxpoints;
				s += sarr[a];
			}
			else
			{
				s = 0;
				std::cout << "train stretches over too many points" << std::endl;
			}
		}
		cx[i] = xarr[a] + s * data::xrel[oarr[a]] / 1000;
		cy[i] = yarr[a] + s * data::yrel[oarr[a]] / 1000;
		qarr[i] = oarr[a];
	}
	//std::cout << "\r" << al << " " << a << "   " << std::flush;
	return;
}