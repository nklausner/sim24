#include "trainManager.h"


TrainManager::TrainManager(SDL_Renderer* renderer_, SDL_Writer* mywriter_)
{
	renderer = renderer_;
	mywriter = mywriter_;
	fill_series_length();

	//loading all xml files from given directory
	//reads content in string-vector
	std::string my_dirname = data::my_directory + "save";
	//for (const auto & entry : std::experimental::filesystem::directory_iterator(my_dirname))
	//std::string my_filename = entry.path().generic_string();
	std::string my_filename = my_dirname + "\\S-Bahn-Stuttgart.xml";
	std::vector<std::string> my_vec = read_file(my_filename);
	extract_timetables(my_vec, data::timetables);
	extract_trains(my_vec, all_trains);

	//Train train1(7510.0, -4817.0, 7596, -4860, 7464, -4794); //U7 Enkheim
	//Train train2("791", -20200.0, 23468.0, -20336, 23536, -20108, 23422); //S1 Herrenberg
	//all_trains.push_back(train1);
	//all_trains.push_back(train2);
	return;
}


void TrainManager::update()
{
	if (true) //data::time % 5000 == 0
	{
		for (Train& t : all_trains)
		{
			t.update();
			//t.update_cars();
		}
	}
	return;
}


void TrainManager::draw_all_trains()
{
	if (renderer && mywriter)
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x66, 0xFF);

		if (data::zoom <= 50)
		{
			draw_all_trains_wagons();
		}
		else
		{
			draw_all_trains_single();
		}
	}
	return;
}


void TrainManager::draw_all_trains_wagons()
{
	std::vector<float> xvec = { 0.0f };
	std::vector<float> yvec = { 0.0f };
	std::vector<int> ovec = { 0 };
	std::vector<float> lvec = { 0.0f };
	//already_debug_one = false;
	for (Train& t : all_trains)
	{
		if (t.is_visible())
		{
			t.set_train_car_coords(xvec, yvec, ovec, lvec);

			if (data::zoom == 50)
			{
				for (unsigned i = 0; i < ovec.size(); i++)
				{
					draw_square(xvec[i], yvec[i]);
				}
			}
			else
			{
				for (unsigned i = 0; i < ovec.size(); i++)
				{
					draw_rectangle(xvec[i], yvec[i], lvec[i] - 1.0f, 3.0f, ovec[i]);
				}
			}
			//if (already_debug_one == false)
			//{
			//	already_debug_one = true;
			//	std::cout << lvec[0] << std::endl;
			//}
		}
	}
	return;
}


void TrainManager::draw_all_trains_single()
{
	for (Train& t : all_trains)
	{
		if (t.is_visible())
		{
			draw_square((float)t.x, (float)t.y);
		}
	}
	return;
}


void TrainManager::draw_all_trains_line()
{
	return;
}


void TrainManager::draw_train_rectangle(Train& t)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x66, 0xFF);
	for (int i = 0; i < t.length; i++)
	{
		//float x = 10.0f * (t.cx[i] - data::xl) / data::zoom;
		//float y = 10.0f * (t.cy[i] - data::yu) / data::zoom;
		//float lh = 5.0f * (t.larr[i] - 1.0f) / data::zoom;
		//float wh = 15.0f / data::zoom;
		//int o = t.qarr[i];
		//draw_rectangle(x, y, lh, wh, o);
		//std::cout << " " << x << " " << y << std::endl;
	}
	return;
}


void TrainManager::draw_square(float xd, float yd)
{
	//draws fixed square around given map positions
	//translates to screen position
	int x = ((int)(10 * xd) - (10 * data::xl)) / data::zoom;
	int y = ((int)(10 * yd) - (10 * data::yu)) / data::zoom;
	SDL_RenderDrawLine(renderer, x - 1, y - 1, x - 1, y + 1);
	SDL_RenderDrawLine(renderer, x - 1, y + 1, x + 1, y + 1);
	SDL_RenderDrawLine(renderer, x + 1, y + 1, x + 1, y - 1);
	SDL_RenderDrawLine(renderer, x + 1, y - 1, x - 1, y - 1);
	return;
}


void TrainManager::draw_rectangle(float x, float y, float lh, float wh, int o)
{
	//draws zoom adjusted rectangle at given map positions
	//translates to screen position
	//length and height are halved for easier calculation from center
	x = (10 * x - 10 * data::xl) / data::zoom;
	y = (10 * y - 10 * data::yu) / data::zoom;
	lh = (5 * lh) / data::zoom;
	wh = (5 * wh) / data::zoom;
	float xo = (float)geom::xrel[o];
	float yo = (float)geom::yrel[o];
	int ax = (int)(x + (lh * xo + wh * yo));
	int ay = (int)(y + (lh * yo - wh * xo));
	int bx = (int)(x + (lh * xo - wh * yo));
	int by = (int)(y + (lh * yo + wh * xo));
	int cx = (int)(x - (lh * xo + wh * yo));
	int cy = (int)(y - (lh * yo - wh * xo));
	int dx = (int)(x - (lh * xo - wh * yo));
	int dy = (int)(y - (lh * yo + wh * xo));
	SDL_RenderDrawLine(renderer, ax, ay, bx, by);
	SDL_RenderDrawLine(renderer, cx, cy, dx, dy);
	SDL_RenderDrawLine(renderer, bx, by, cx, cy);
	SDL_RenderDrawLine(renderer, dx, dy, ax, ay);
	return;
}