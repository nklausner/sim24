#include "drawer.h"


Drawer::Drawer(SDL_Renderer* renderer_, SDL_Writer* mywriter_)
{
	renderer = renderer_;
	mywriter = mywriter_;
	return;
}


void Drawer::update_coords()
{
	using namespace data;
	switch (command)
	{
	case SCROLL_LE: x -= 10 * zoom; break;
	case SCROLL_RI: x += 10 * zoom; break;
	case SCROLL_UP: y -= 10 * zoom; break;
	case SCROLL_DO: y += 10 * zoom; break;
	case ZOOM_IN:   increase_zoom(); break;
	case ZOOM_OUT:  decrease_zoom(); break;
	}
	xl = x - w * zoom / 20;
	xr = x + w * zoom / 20;
	yu = y - h * zoom / 20;
	yd = y + h * zoom / 20;
	tldxl = xl - tld;
	tldxr = xr + tld;
	tldyu = yu - tld;
	tldyd = yd + tld;
	xt0 = get_tile_index(data::xl);
	xt1 = get_tile_index(data::xr);
	yt0 = get_tile_index(data::yu);
	yt1 = get_tile_index(data::yd);

	active_tiles.clear();
	for (int xt = xt0; xt <= xt1; xt++)
	{
		for (int yt = yt0; yt <= yt1; yt++)
		{
			Tile* t = find_tile(xt, yt, false);
			if (t) {
				active_tiles.push_back(t);
			}
		}
	}
	//std::cout << "updating: tiles " << active_tiles.size() << std::endl;
	//std::cout << "updated borders " << tldxl << " " << tldxr << " " << tldyu << " " << tldyd << std::endl;
	return;
}


void Drawer::increase_zoom()
{
	using namespace data;
	for (int i = 0; i < 4; i++)
	{
		if (zoom == zoomarr[i]) {
			zoom = zoomarr[i + 1];
			break;
		}
	}
	return;
}


void Drawer::decrease_zoom()
{
	using namespace data;
	for (int i = 1; i < 5; i++)
	{
		if (zoom == zoomarr[i]) {
			zoom = zoomarr[i - 1];
			break;
		}
	}
	return;
}


void Drawer::draw()
{
	if (renderer && mywriter)
	{
		//Render background
		SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 0xFF);
		SDL_RenderClear(renderer);

		//Render tile grid
		SDL_SetRenderDrawColor(renderer, 0xDD, 0xDD, 0xDD, 0xFF);
		draw_tile_grid();

		//Render some lines for rails
		SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0x66, 0xFF);
		draw_rails();

		//Render some lines for platforms
		if (data::zoom <= 50)
		{
			SDL_SetRenderDrawColor(renderer, 0x99, 0x88, 0x77, 0xFF);
			draw_platforms();
			draw_labels();
		}


		//Update screen
		//SDL_RenderPresent(renderer);
	}
	return;
}


void Drawer::draw_rails()
{
	//loop over all vectors of all tiles and draw the rails
	data::drawn_rails = 0;
	for (Tile* t : active_tiles)
	{
		for (unsigned j = 0; j < t->rails.size(); j++)
		{
			for (unsigned i = 1; i < t->rails[j].size(); i++)
			{
				draw_line(t->rails[j][i - 1], t->rails[j][i]);
				if (t->rails[j][i].findex < 4294967295)
				{
					draw_square(t->rails[j][i]);
				}
				data::drawn_rails++;
			}
		}
	}
	return;
}


void Drawer::draw_platforms()
{
	//loop over all vectors of the tile and draw the platforms
	data::drawn_platforms = 0;
	for (Tile* t : active_tiles)
	{
		for (unsigned j = 0; j < t->platforms.size(); j++)
		{
			for (unsigned i = 1; i < t->platforms[j].size(); i++)
			{
				draw_line(t->platforms[j][i - 1], t->platforms[j][i]);
				data::drawn_platforms++;
			}
			draw_line(t->platforms[j].back(), t->platforms[j].front());
			data::drawn_platforms++;
		}
	}
	return;
}


void Drawer::draw_labels()
{
	//loop over all labels of all tiles and draw
	data::drawn_labels = 0;
	for (Tile* t : active_tiles)
	{
		for (Label& mylabel : t->labels)
		{
			int x = ((mylabel.x - data::xl) * 10) / data::zoom;
			int y = ((mylabel.y - data::yu) * 10) / data::zoom;
			mywriter->write_center(mylabel.s, x, y);
			data::drawn_labels++;
		}
	}
	return;
}


void Drawer::draw_tile_grid()
{
	//loop over all tiles to draw
	int xt0 = get_tile_index(data::xl);
	int xt1 = get_tile_index(data::xr);
	int yt0 = get_tile_index(data::yu);
	int yt1 = get_tile_index(data::yd);
	int x = 0;
	int y = 0;

	for (int xt = xt0 + 1; xt <= xt1; xt++)
	{
		x = (xt * data::s - data::xl) * 10 / data::zoom;
		SDL_RenderDrawLine(renderer, x, 0, x, data::h);
	}
	for (int yt = yt0 + 1; yt <= yt1; yt++)
	{
		y = (yt * data::s - data::yu) * 10 / data::zoom;
		SDL_RenderDrawLine(renderer, 0, y, data::w, y);
	}
	return;
}


//int Drawer::get_tile_number(int x)
//{
//	return (x < 0) ? (x / data::s) - 1 : (x / data::s);
//}


void Drawer::draw_line(Point& p0, Point& p1)
{
	int x1 = (p0.x - data::xl) * 10 / data::zoom;
	int y1 = (p0.y - data::yu) * 10 / data::zoom;
	int x2 = (p1.x - data::xl) * 10 / data::zoom;
	int y2 = (p1.y - data::yu) * 10 / data::zoom;
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	return;
}


void Drawer::draw_line(Pointf& p0, Pointf& p1)
{
	int x1 = (int)((p0.x - data::xl) * 10) / data::zoom;
	int y1 = (int)((p0.y - data::yu) * 10) / data::zoom;
	int x2 = (int)((p1.x - data::xl) * 10) / data::zoom;
	int y2 = (int)((p1.y - data::yu) * 10) / data::zoom;
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	return;
}


void Drawer::draw_square(Point& p)
{
	int x = 10 * (p.x - data::xl) / data::zoom;
	int y = 10 * (p.y - data::yu) / data::zoom;
	SDL_RenderDrawLine(renderer, x - 3, y - 3, x - 3, y + 3);
	SDL_RenderDrawLine(renderer, x - 3, y + 3, x + 3, y + 3);
	SDL_RenderDrawLine(renderer, x + 3, y + 3, x + 3, y - 3);
	SDL_RenderDrawLine(renderer, x + 3, y - 3, x - 3, y - 3);
	return;
}