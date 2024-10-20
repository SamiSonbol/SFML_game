#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"

using namespace sf;

class Fighter
{
public :

	bool is_left;

	bool is_on_ground;

	bool is_blocking;

	bool already_picked;

	bool was_chosen;

	bool dead;

	bool is_attacking;

	bool Player_is_attacking;

	bool stationary;

	bool is_hit;

	bool Player_is_moving;

	bool Player_has_attacked;

	float movementSpeed;

	float window_height;

	float window_width;

	Vector2f velocity;

	Event my_event;

	sf::Sprite main_sprite;

	Animator animator;

	Fighter() {};

	//virtual void idleState() = 0;
	
	virtual void movement(const float& delta_time) = 0;
	virtual void combat(const float& delta_time) = 0;

	void update_sprite(Spritesheet& spritesheet, sf::Sprite& sprite, const sf::Vector2f& sprite_scale, const float& delta_time);

	void screen_collision();

	//i opted to use 2 bool functions instead of negating 1 bool function inorder to avoid edge cases and keep syntax clear
	bool is_on_right_side(const sf::Sprite& sprite);
	bool is_on_left_side(const sf::Sprite& sprite);

	bool intersects(const sf::Sprite& sprite);
	bool contains(const sf::Vector2f& pos);

	void fighter_collision(Fighter& enemy_fighter);

	bool in_range(const float range, const sf::Vector2f& target);

	void RENDER(sf::RenderWindow* window, const float& delta_time);

};



