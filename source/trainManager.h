#pragma once
#include "train.h"
#include <SDL.h>
#include "SDL_writer.h"
#include <vector>


//2022-07-03: initialize train manager


struct TrainManager
{
public:
	TrainManager(SDL_Renderer* renderer, SDL_Writer* mywriter);
	void update();
	void draw_all();

private:
	void draw_train_square(Train& t);
	void draw_train_rectangle(Train& t);
	std::vector<Train> all_trains;

	SDL_Renderer* renderer = NULL;
	SDL_Writer* mywriter = NULL;
	void draw_square(int x, int y);
	void draw_rectangle(float x, float y, float lh, float wh, int o);

};