#pragma once
#include <SFML/Graphics.hpp>
#include<memory>

using namespace sf;

class Background
{
public:

	float time;

	Clock time_clock;

	bool is_time_set;

	int stage;

	int flip_x;

	int flip_y;

	Texture background;//-----------background texture bamboo forest
	
	Sprite moon_bamboo_forest;
	
	Background(int stage);

	void load_background_frames(int stage);

};

