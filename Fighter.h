#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Fighter
{
public :

	Texture idle_looking_AtLeftside;

	Texture idle;

	Texture idle_left;

	Texture attack_from_rightside1;

	Texture attack_from_leftside1;

	Texture attack_from_rightside2;

	Texture attack_from_leftside2;

	Texture attack_from_rightside3;

	Texture attack_from_leftside3;

	Texture run_to_rightside;

	Texture run_to_leftside;

	Texture jump_to_rightside;

	Texture jump_to_leftside;

	Texture block_to_rightside;

	Texture block_to_leftside;

	Texture hurt_at_leftside;

	Texture hurt_at_rightside;

	Texture dead_from_rightside;

	Texture dead_from_leftside;

	Sprite idle_sprite;

	int flip;

	int flip_toLeft;

	int flip_attack;

	int flip_attack_from_rightside;

	int flip_idle;

	int when_to_fallX;

	int when_to_fallY;

	int flip_block;

	int flip_dead_from_rightside;

	int flip_dead_from_leftside;

	bool is_left;

	bool is_on_ground;

	bool is_blocking;

	bool already_picked;

	bool was_chosen;

	bool dead;

	bool is_attacking;

	bool Player_is_on_rightside;

	bool Player_is_attacking;

	bool stationary;

	bool is_hit;

	bool Player_is_moving;

	bool Player_has_attacked;

	float movementSpeed;

	float dt;

	float window_height;

	float window_width;

	Vector2f velocity;

	Vector2f pos;

	Vector2f Player_pos;

	Event my_event;

	Clock dt_clock;

	Fighter() {};

	//virtual void idleState() = 0;
	
	virtual void movement() = 0;
	virtual void combat() = 0;

	void screen_collision();

	template <typename U>
    void fighter_collision(std::shared_ptr<U> EnemyFighter);

};



