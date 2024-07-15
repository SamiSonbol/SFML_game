#include "Background.h"

Background::Background(int stage) {

	this->stage = stage;

	time = 0.f;

	time_clock;

	is_time_set = false;

	flip_x = 0;

	flip_y = 0;

	if (stage == 12) {

		background.loadFromFile("moon_bamboo_forest.png");

		moon_bamboo_forest.setTexture(background);

		moon_bamboo_forest.setTextureRect(IntRect(flip_x, flip_y, 800, 335));

	    moon_bamboo_forest.setScale(2.4, 3.22);


	}

}

void Background::load_background_frames(int stage) {

	this->stage = stage;

	if (stage == 12) {

		is_time_set = true;

		if (is_time_set) {

			time += time_clock.restart().asMilliseconds();

			if (time >= 200.f) {

				moon_bamboo_forest.setTextureRect(IntRect(flip_x, flip_y, 800, 335));

				flip_x = flip_x + 800;

		        moon_bamboo_forest.setTextureRect(IntRect(flip_x, flip_y, 800, 335));

				if (flip_x >= 3200) {

					flip_x = 0;

					flip_y = flip_y + 336;
				}

				if (flip_y >= 2353) {

					flip_y = 0;

				}

			time = 0.f;

			is_time_set = false;

			}

		}
		
	}

}
