#include "train.h"


Train::Train(std::string line_, std::string role_, std::string comp_, std::string start_, int x0, int y0, int o0)
{
	status = States::STOPPED;
	line = line_;
	role = role_;
	comp = comp_;
	orig = "";
	dest = "";
	linedest = line_;
	set_wagon_spacing(comp_);

	//set the location properties
	x = double(x0);
	y = double(y0);
	o = o0;
	s = 0.0;

	//set the first segment in the point arrays
	al = 0;
	an = 1;
	xarr[an] = x0;
	yarr[an] = y0;
	xarr[al] = x0 - int(1000.0 * geom::xrel[o0]);
	yarr[al] = y0 - int(1000.0 * geom::yrel[o0]);
	oarr[an] = o;
	sarr[an] = get_distance(xarr[an] - xarr[al], yarr[an] - yarr[al], o);

	//set tile properties
	xt = get_tile_index((int)x);
	yt = get_tile_index((int)y);
	tile = find_tile(xt, yt, false);

	v = 0.0;
	vtar = 0.0;
	vall = 60.0 / 3.6;
	vmax = 140.0 / 3.6;
	//oarr[al] = find_orientation(xarr[an] - xarr[al], yarr[an] - yarr[al]);
	//qarr[0] = oarr[al];
	//sarr[al] = get_distance(xarr[an] - xarr[al], yarr[an] - yarr[al], oarr[al]);
	//sarr[an] = get_distance(xarr[an] - (int)cx[0], yarr[an] - (int)cy[0], oarr[al]);

	t = data::time;
	tstart = find_departure(start_);
	tstop = 45;
	//2 * data::time; // data::time + 1000;

	acc = 0.6;
	dec = 0.6;

	//print debug stuff
	//std::cout << "created train: " << line << " at " << x << "," << y << "," << o << std::endl;
	//std::cout << larr[0] << std::endl;
	return;
}


void Train::update()
{
	iupdate++;
	double dt = (double)(data::time - t) / 1000.0; //diff time in seconds
	double dv = 0.0;		//diff velocity in meter/seconds
	double ds = 0.0;		//diff distance in meters
	//float dt = data::speedfactor * (data::time - tlast);
	//std::cout << dt << std::endl;

	if (tile == NULL)
	{
		return;
	}
	//std::cout << status << std::endl;
	if (status == States::STOPPED)
	{
		if (data::time > tstart)
		{
			vtar = vall;
			t = tstart;
			status = States::ACCELERATING;
			//std::cout << "departure now" << std::endl;
			update();
			return;
		}
	}
	else if (status == States::COASTING)
	{
		ds = v * dt;
	}
	else if (status == States::ACCELERATING)
	{
		dv = acc * dt;
		if (v + dv > vtar)
		{
			dv = vtar - v;
			status = States::COASTING;
		}
		ds = (v + dv / 2.0) * dt;
	}
	else if (status == States::DECELERATING)
	{
		dv = -1 * (dec * dt);
		if (v + dv < vtar)
		{
			dv = vtar - v;
			status = States::COASTING;
		}
		ds = (v + dv / 2.0) * dt;
	}

	if (ds > 0)
	{
		if (ds <= s)
		{
			//case: enough strech left on the current segment
			//just shift along the segment
			t = data::time;
			v += dv;
			x += ds * geom::xrel[o];
			y += ds * geom::yrel[o];
			s -= ds;
			supdate += ds;
			//std::cout << ds * geom::xrel[o] << ", " << ds * geom::yrel[o] << std::endl;
		}
		else
		{
			//std::cout << "find next segment" << std::endl;
			//case: reaching the end of a segment
			//calculate values at the current next point
			//tolerate errors: time rounded, acc/dec approximated
			//std::cout << "fraction done: " << s / ds << std::endl;
			t += int(1000 * dt * (s / ds));
			v += (s / ds) * dv;
			x = (double)xarr[an];
			y = (double)yarr[an];
			supdate += s;

			//-1 means no orientation defined by switch
			oswitch = -1;
			size_t findex = 4294967295;
			if (ivec + ilast + inext > 0)
			{
				findex = tile->rails[ivec][inext].findex;
			}
			//case: switch at the end of segment
			if (tile && findex != 4294967295)
			{
				if (tile->functions[findex].get_type() == SWITCH)
				{
					oswitch = tile->functions[findex].decide_orientation(line, linedest, o);
				}
				else if (tile->functions[findex].get_orientation() == o)
				{
					if (tile->functions[findex].get_type() == TIMETABLE)
					{
						std::string tt_name = tile->functions[findex].get_name();
						stop_till(find_departure(tt_name));
					}
					else if (tile->functions[findex].get_type() == STOP)
					{
						stop_for(tile->functions[findex].get_time() * 1000);
					}
					else if (tile->functions[findex].get_type() == SPEED)
					{
						bool slowing = (vtar < vall) ? true : false;
						vall = (double)tile->functions[findex].get_speed() / 3.6;
						vall = (vall < vmax) ? vall : vmax;
						if (vall > v && !slowing)
						{
							vtar = vall;
							status = States::ACCELERATING;
						}
						if (vall < v)
						{
							vtar = vall;
							status = States::DECELERATING;
						}
						//std::cout << line << " changing speed to " << vtar << " state " << status << std::endl;
					}
					else if (tile->functions[findex].get_type() == REVERSE)
					{
						reverse_front();
						stop_for(15 * 1000);
						update();
						return;
					}
				}
			}

			//find and set following next point
			find_next_point();

			if (tile)
			{
				//recursive calling for every additional segment
				update();
				return;
			}
		}
	}
	//update time anyway at the end
	t = data::time;
	//std::cout << iupdate << " " << supdate << std::endl;
	supdate = 0.0;
	iupdate = 0;
	return;
}


void Train::find_next_point()
{
	//std::cout << "find next point, array: " << an << " " << al << ", segm: " << inext << " " << ilast << std::endl;
	//update array indices (last and next)
	al = an;
	an = (an + 1) % maxpoints;

	if (oswitch == -1 && inext > ilast && inext < tile->rails[ivec].size() - 1)
	{
		//going in positive direction along path
		inext++;
		ilast++;
	}
	else if (oswitch == -1 && inext < ilast && inext > 0)
	{
		//going in negative direction along path
		inext--;
		ilast--;
	}
	else
	{
		//need to change path or even tile
		tile = find_next_tile();
	}
	if (tile)
	{
		//prepare new segment on (eventually) new tile 
		xt = tile->xt;
		yt = tile->yt;
		xarr[an] = tile->rails[ivec][inext].x;
		yarr[an] = tile->rails[ivec][inext].y;
		o = get_orientation(xarr[an] - xarr[al], yarr[an] - yarr[al]);
		s = get_distance(xarr[an] - xarr[al], yarr[an] - yarr[al], o);
		oarr[an] = o;
		sarr[an] = s;
		//std::cout << "segment: " << xarr[an] << ", " << yarr[an] << ", " << ilast << ", " << inext << ", " << o << ", " << s << std::endl;
		//std::cout << "segment: " << xarr[an] << ", " << yarr[an] << ", " << tile->rails[ivec][inext].findex << std::endl;
	}
	else
	{
		std::cout << "[ERROR] next point not found at: " << x << ", " << y << std::endl;
	}
	return;
}


Tile* Train::find_next_tile()
{
	//std::cout << "find next tile: " << xarr[al] << " " << yarr[al] << std::endl;
	//search in own and neighboring tiles
	//search all rails and all points
	//for a subsequent segment with fitting orientation
	int dxt[9] = { 0, +1, -1, 0, 0, +1, -1, -1, +1 };
	int dyt[9] = { 0, 0, 0, +1, -1, +1, -1, +1, -1 };

	for (int k = 0; k < 9; k++)
	{
		Tile* tnew = find_tile(xt + dxt[k], yt + dyt[k], false);
		if (tnew)
		{
			for (int j = 0; j < tnew->rails.size(); j++)
			{
				for (int i = 0; i < tnew->rails[j].size(); i++)
				{
					if (tnew->rails[j][i].x == xarr[al] &&
						tnew->rails[j][i].y == yarr[al])
					{
						//if (tnew->rails[j][i].x == -12700 && tnew->rails[j][i].y == +14700)
						//{
							//std::cout << "[SEARCH] " << al << ", " << an << std::endl;
							//std::cout << "[SEARCH] " << ilast << ", " << inext << std::endl;
							//std::cout << "[SEARCH] " << tnew->xt << "," << tnew->yt << " rails " << tnew->rails.size() << std::endl;
							//std::cout << "[SEARCH] " << j << " " << tnew->rails[j].size() << " " << i << std::endl;
						//}
						//if (tnew->rails[j][i].x == -20110 && tnew->rails[j][i].y == +23417)
						//{
						//	std::cout << "[SEARCH] found a point in the tile" << std::endl;
						//	std::cout << ivec << " " << inext << " " << ilast << " --- " << oswitch << std::endl;
						//}
						if (i < tnew->rails[j].size() - 1)
						{
							//ascending indices
							int dx = tnew->rails[j][i + 1].x - tnew->rails[j][i].x;
							int dy = tnew->rails[j][i + 1].y - tnew->rails[j][i].y;
							int onew = get_orientation(dx, dy);
							if ((oswitch != -1 && onew == oswitch) ||
								(oswitch == -1 && is_allowed_turn(o, onew)))
							{
								ivec = j;
								ilast = i;
								inext = i + 1;
								return tnew;
							}
						}
						if (i > 0)
						{
							//descending indices
							int dx = tnew->rails[j][i - 1].x - tnew->rails[j][i].x;
							int dy = tnew->rails[j][i - 1].y - tnew->rails[j][i].y;
							int onew = get_orientation(dx, dy);
							if ((oswitch != -1 && onew == oswitch) ||
								(oswitch == -1 && is_allowed_turn(o, onew)))
							{
								ivec = j;
								ilast = i;
								inext = i - 1;
								//std::cout << tnew->rails[j].size() << std::endl;
								//std::cout << i << ", " << ilast << ", " << inext << std::endl;
								return tnew;
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}


bool Train::is_visible()
{
	if ((int)x < data::tldxl || (int)x > data::tldxr ||
		(int)y < data::tldyu || (int)y > data::tldyd)
	{
		return false;
	}
	return true;
}


void Train::set_train_car_coords(std::vector<float>& xvec, std::vector<float>& yvec, std::vector<int>& ovec, std::vector<float>& lvec)
{
	xvec.clear();
	yvec.clear();
	ovec.clear();
	lvec.clear();
	unsigned ai = al;
	float si = (float)(sarr[an] - s);
	int oi = o;
	//std::cout << "- " << ai << " " << si << " ";

	for (unsigned i = 0; i < count; i++)
	{
		while (darr[i] > si && si >= 0)
		{
			oi = oarr[ai];
			si += (float)sarr[ai];
			ai = subcyc(ai, maxpoints);
			//std::cout << "- " << ai << " " << si << " ";
		}
		xvec.push_back(((float)xarr[ai] + (si - darr[i]) * (float)geom::xrel[oi]));
		yvec.push_back(((float)yarr[ai] + (si - darr[i]) * (float)geom::yrel[oi]));
		ovec.push_back(oi);
		lvec.push_back(larr[i]);
	}
	//std::cout << " " << std::endl;
	return;
}


int Train::find_departure(std::string& my_name)
{
	//search for timetable by name and returns next available departure time in milliseconds
	int my_dep = data::time + 1000;
	for (auto& mytt : data::timetables)
	{
		if (mytt.get_name() == my_name)
		{
			my_dep = mytt.get_departure(data::time);
			linedest = mytt.get_destination_line(line);
			ttlast = my_dep;
			break;
		}
	}
	//std::cout << "departure: " << my_name << " " << my_dep/60000 << std::endl;
	return my_dep;
}


void Train::set_wagon_spacing(std::string& my_comp)
{
	//set wagon positions as distance from front, wagon count, total length
	std::string myentry;
	std::stringstream mystream(my_comp);
	count = 0;
	length = 0.0f;
	float l = 0.0f;
	while (std::getline(mystream, myentry, ' ') && count < 32)
	{
		l = data::series_length[myentry];
		larr[count] = l;
		darr[count] = length + l / (float)2.0;
		length += l;
		//std::cout << larr[count] << ' ';
		count++;
	}
	//std::cout << "end " << length << std::endl;
	return;
}


void Train::reverse_front()
{
	//create new arrays
	int xnew[maxpoints] = { 0 };
	int ynew[maxpoints] = { 0 };
	int onew[maxpoints] = { 0 };
	double snew[maxpoints] = { 0.0 };
	//start with the last point
	double si = 0;
	int ar = an;
	unsigned i = 0;
	//fill first point
	xnew[0] = xarr[an];
	ynew[0] = yarr[an];
	//fill new arrays as long as length not reached
	while (si < length && i < maxpoints)
	{
		i++;
		onew[i] = (oarr[ar] + 12) % 24;
		snew[i] = sarr[ar];
		si += sarr[ar];
		ar = subcyc(ar, maxpoints);
		xnew[i] = xarr[ar];
		ynew[i] = yarr[ar];
	}
	//set other new values
	an = i;
	al = i - 1;
	s = si - length;
	o = onew[i];
	x = (double)xnew[i] - s * geom::xrel[o];
	y = (double)ynew[i] - s * geom::yrel[o];
	v = 0.0;
	//set tile properties
	xt = get_tile_index((int)x);
	yt = get_tile_index((int)y);
	tile = find_tile(xt, yt, false);
	ivec = 0;
	ilast = 0;
	inext = 0;
	//overwrite arrays
	for (unsigned i = 0; i < maxpoints; i++)
	{
		xarr[i] = xnew[i];
		yarr[i] = ynew[i];
		oarr[i] = onew[i];
		sarr[i] = snew[i];
		//std::cout << xarr[i] << " " << yarr[i] << " " << oarr[i] << " " << sarr[i] << std::endl;
	}
	if (tile)
	{
		find_current_segment();
	}
	std::cout << line << " reverse " << t / 3600000 % 24 << ":" << t / 60000 % 60 << ":" << t / 1000 % 60 << std::endl;
	//std::cout << "train turned around " << xarr[an] << " " << yarr[an] << " " << o << std::endl;
	return;
}


void Train::find_current_segment()
{
	//find segment indices if only positions (array) are given
	//needed after reversing
	//std::cout << "finding current segment at: " << xarr[an] << " " << yarr[an] << std::endl;
	for (int j = 0; j < tile->rails.size(); j++)
	{
		for (int i = 0; i < tile->rails[j].size(); i++)
		{
			if (tile->rails[j][i].x == xarr[an] &&
				tile->rails[j][i].y == yarr[an])
			{
				//case: found the current next point
				//try both: previous and next segment
				if (i > 0)
				{
					if (tile->rails[j][i - 1].x == xarr[al] &&
						tile->rails[j][i - 1].y == yarr[al])
					{
						ivec = j;
						ilast = i - 1;
						inext = i;
						//std::cout << "found current segment: " << ivec << " " << ilast << " " << inext << std::endl;
						return;
					}
				}
				if (i + 1 < tile->rails[j].size())
				{
					if (tile->rails[j][i + 1].x == xarr[al] &&
						tile->rails[j][i + 1].y == yarr[al])
					{
						ivec = j;
						ilast = i + 1;
						inext = i;
						//std::cout << "found current segment: " << ivec << " " << ilast << " " << inext << std::endl;
						return;
					}
				}
			}
		}
	}
	return;
}


void Train::stop_for(int my_time)
{
	//put the train into stop state for given duration in milliseconds
	stop_till(t + my_time);
	return;
}
void Train::stop_till(int my_time)
{
	//put the train into stop state till starttime in milliseconds
	status = States::STOPPED;
	v = 0.0;
	tstart = my_time;
	return;
}