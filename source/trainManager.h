#pragma once
#include <SDL.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
//#include <filesystem>
#include "data.h"
#include "train.h"
#include "fileHandler.h"
#include "SDL_writer.h"
#include "trainLoader.h"


//2022-07-03: initialize train manager


struct TrainManager
{
public:
	TrainManager(SDL_Renderer* renderer, SDL_Writer* mywriter);
	void update();
	void draw_all_trains();

private:
	//reference to SDL objects
	SDL_Renderer* renderer = NULL;
	SDL_Writer* mywriter = NULL;
	//the vector holding all train objects
	std::vector<Train> all_trains;

	void draw_all_trains_wagons();
	void draw_all_trains_single();
	void draw_all_trains_line();
	void draw_train_rectangle(Train& t);

	void draw_square(float x, float y);
	void draw_rectangle(float x, float y, float l, float w, int o);

	bool already_debug_one = true;
};