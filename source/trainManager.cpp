#include "trainManager.h"


TrainManager::TrainManager(SDL_Renderer* renderer_, SDL_Writer* mywriter_)
{
	renderer = renderer_;
	mywriter = mywriter_;
	all_trains.clear();
	Train train1(7530.0, -4827.0, 7596, -4860, 7464, -4794);
	all_trains.push_back(train1);
	return;
}


void TrainManager::update()
{
	for (Train& t : all_trains)
	{
		t.update();
		t.update_cars();
	}
	return;
}


void TrainManager::draw_all()
{
	if (renderer && mywriter)
	{
		if (data::zoom > 10)
		{
			for (Train& t : all_trains)
			{
				draw_train_square(t);
			}
		}
		else
		{
			for (Train& t : all_trains)
			{
				draw_train_rectangle(t);
			}
		}
	}
	return;
}


void TrainManager::draw_train_square(Train& t)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x66, 0xFF);
	for (int i = 0; i < t.length; i++)
	{
		int x = 10 * ((int)t.cx[i] - data::xl) / data::zoom;
		int y = 10 * ((int)t.cy[i] - data::yu) / data::zoom;
		draw_square(x, y);
		//std::cout << " " << x << " " << y << std::endl;
	}
	return;
}


void TrainManager::draw_train_rectangle(Train& t)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x66, 0x66, 0xFF);
	for (int i = 0; i < t.length; i++)
	{
		float x = 10.0f * (t.cx[i] - data::xl) / data::zoom;
		float y = 10.0f * (t.cy[i] - data::yu) / data::zoom;
		float lh = 5.0f * (t.larr[i] - 1.0f) / data::zoom;
		float wh = 15.0f / data::zoom;
		int o = t.qarr[i];
		draw_rectangle(x, y, lh, wh, o);
		//std::cout << " " << x << " " << y << std::endl;
	}
	return;
}


void TrainManager::draw_square(int x, int y)
{
	SDL_RenderDrawLine(renderer, x - 2, y - 2, x - 2, y + 2);
	SDL_RenderDrawLine(renderer, x - 2, y + 2, x + 2, y + 2);
	SDL_RenderDrawLine(renderer, x + 2, y + 2, x + 2, y - 2);
	SDL_RenderDrawLine(renderer, x + 2, y - 2, x - 2, y - 2);
	return;
}


void TrainManager::draw_rectangle(float x, float y, float lh, float wh, int o)
{
	int xo = data::xrel[o];
	int yo = data::yrel[o];
	int ax = (int)(x + (lh * xo + wh * yo) / 1000);
	int ay = (int)(y + (lh * yo - wh * xo) / 1000);
	int bx = (int)(x + (lh * xo - wh * yo) / 1000);
	int by = (int)(y + (lh * yo + wh * xo) / 1000);
	int cx = (int)(x - (lh * xo + wh * yo) / 1000);
	int cy = (int)(y - (lh * yo - wh * xo) / 1000);
	int dx = (int)(x - (lh * xo - wh * yo) / 1000);
	int dy = (int)(y - (lh * yo + wh * xo) / 1000);
	SDL_RenderDrawLine(renderer, ax, ay, bx, by);
	SDL_RenderDrawLine(renderer, cx, cy, dx, dy);
	SDL_RenderDrawLine(renderer, bx, by, cx, cy);
	SDL_RenderDrawLine(renderer, dx, dy, ax, ay);
	return;
}