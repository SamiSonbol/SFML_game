#include "Fighter.h"

void Fighter::screen_collision() {

	//collision right
	if (pos.x + idle_sprite.getGlobalBounds().width > window_width) {

		idle_sprite.setPosition(window_width - idle_sprite.getGlobalBounds().width, pos.y);

		Player_is_on_rightside = true;

	}//collision left
	if (pos.x < 0) {

		idle_sprite.setPosition(0, pos.y);

		Player_is_on_rightside = false;

	}//collision bottom
	if (pos.y + idle_sprite.getGlobalBounds().height > window_height) {

		idle_sprite.setPosition(pos.x, window_height - idle_sprite.getGlobalBounds().height);

	}//collision top
	if (pos.y < 0) {

		idle_sprite.setPosition(pos.x, 0);
	}

}
