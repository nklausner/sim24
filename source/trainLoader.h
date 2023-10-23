#pragma once
#include "data.h"
#include "train.h"
#include "timetable.h"
#include "fileHandler.h"


//2023-06-05: initiate train-loader


//struct TrainLoader
//{
//public:
//	TrainLoader();
//private:
//};

void extract_timetables(std::vector<std::string>& my_raw, std::vector<Timetable>& my_timetables);
void extract_trains(std::vector<std::string>& my_raw, std::vector<Train>& my_trains);
void position_train();
void fill_series_length();