#include "tileAnalyzer.h"


TileAnalyzer::TileAnalyzer()
{
	return;
}



void TileAnalyzer::print_current_tile()
{
	//print info of current tile for debugging
	int xt = get_tile_index(data::x);
	int yt = get_tile_index(data::y);
	Tile* mytile = find_tile(xt, yt, false);
	if (mytile)
	{
		std::cout << "TILE " << xt << ", " << yt << std::endl;
		for (auto& myrail : mytile->rails)
		{
			//print out full rail
			std::cout << "==";
			for (Point& mypoint : myrail)
			{
				std::cout << " " << mypoint.x << "," << mypoint.y;
			}
			std::cout << std::endl;

			//print functions on rail
			for (Point& mypoint : myrail)
			{
				if (mypoint.findex < 4294967295)
				{
					//std::cout << functionName[mypoint.findex.getType()];
					std::cout << functionName[mytile->functions[mypoint.findex].get_type()];
					std::cout << " " << mypoint.x << "," << mypoint.y << " " << mypoint.findex;
					std::cout << std::endl;
				}
			}
		}
		std::cout << " " << std::endl;
	}
	return;
}