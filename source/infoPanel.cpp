#include "infoPanel.h"


InfoPanel::InfoPanel(SDL_Renderer* renderer_, SDL_Writer* writer_)
{
	renderer = renderer_;
	writer = writer_;
	return;
}


void InfoPanel::draw()
{
	SDL_Rect infoPanelBackgr = { 0, 0, 200, 100 };
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &infoPanelBackgr);
	SDL_Rect infoPanelBorder = { 0, 0, 200, 100 };
	SDL_SetRenderDrawColor(renderer, 0xDD, 0xDD, 0xDD, 0xFF);
	SDL_RenderDrawRect(renderer, &infoPanelBorder);

	if (data::is_running){
		writer->write(std::to_string(data::speed), 4, 2);
	}
	else {
		writer->write("STOP", 4, 2);
	}
	writer->write(writeTime(data::time / 1000), 54, 2);

	writer->write(writeFloat(data::zoom), 4, 18);
	writer->write(writeFloat(data::x / 100) + "," + writeFloat(data::y / 100), 54, 18);
	return;
}


std::string InfoPanel::writeFloat(int x)
{
	return std::to_string(x / 10) + "." + std::to_string(std::abs(x) % 10);
}


std::string InfoPanel::writeTime(int t)
{
	std::string h = std::to_string(t / 3600 % 24);
	std::string m = std::to_string(t / 60 % 60);
	std::string s = std::to_string(t % 60);
	if (h.length() == 1) { h.insert(0, 1, '0'); }
	if (m.length() == 1) { m.insert(0, 1, '0'); }
	if (s.length() == 1) { s.insert(0, 1, '0'); }
	return h + ":" + m + ":" + s;
}
