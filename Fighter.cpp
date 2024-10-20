#include "Fighter.h"

void Fighter::screen_collision() {

	////collision right
	//if (pos.x + main_sprite.getGlobalBounds().width > window_width) {

	//	main_sprite.setPosition(window_width - main_sprite.getGlobalBounds().width, pos.y);

	//	Player_is_on_rightside = true;

	//}//collision left
	//if (pos.x < 0) {

	//	main_sprite.setPosition(0, pos.y);

	//	Player_is_on_rightside = false;

	//}//collision bottom
	//if (pos.y + main_sprite.getGlobalBounds().height > window_height) {

	//	main_sprite.setPosition(pos.x, window_height - main_sprite.getGlobalBounds().height);

	//}//collision top
	//if (pos.y < 0) {

	//	main_sprite.setPosition(pos.x, 0);
	//}

};

bool Fighter::intersects(const sf::Sprite& sprite) {

	return this->main_sprite.getGlobalBounds().intersects(sprite.getGlobalBounds());

};

bool Fighter::is_on_right_side(const sf::Sprite& sprite) {

	return this->main_sprite.getPosition().x > sprite.getPosition().x;

};

bool Fighter::is_on_left_side(const sf::Sprite& sprite) {

	return this->main_sprite.getPosition().x < sprite.getPosition().x;

};

bool Fighter::contains(const sf::Vector2f& pos) {

	return this->main_sprite.getGlobalBounds().contains(pos);

};

bool Fighter::in_range(const float range, const sf::Vector2f& target) {

	sf::Vector2f direction = target - this->main_sprite.getPosition();
	float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	
	return magnitude <= range;

};

void Fighter::fighter_collision(Fighter& enemy_fighter) {

	sf::Vector2f pos = this->main_sprite.getPosition();

	if (this->intersects(enemy_fighter.main_sprite)) {

		if (this->is_on_right_side(enemy_fighter.main_sprite)) {

			this->main_sprite.setPosition(pos.x + enemy_fighter.main_sprite.getGlobalBounds().width, pos.y);

		}
		else {

			this->main_sprite.setPosition(pos.x - enemy_fighter.main_sprite.getGlobalBounds().width, pos.y);

		};

	};

};

void Fighter::update_sprite(Spritesheet& spritesheet, sf::Sprite& sprite, const sf::Vector2f& sprite_scale, const float& delta_time) {

	this->animator.update(sprite, spritesheet.texture, spritesheet.size, spritesheet.frame_size, sprite_scale, spritesheet.current_frame, spritesheet.elapsed_time, delta_time);

};

void Fighter::RENDER(sf::RenderWindow* window, const float& delta_time) {

    window->draw(this->main_sprite);

};


