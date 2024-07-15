#pragma once
#include "Fighter.h"
#include <SFML/Graphics.hpp>
#include <memory>

using namespace sf;

class Aramusha;

class Enemy;

class SamuraiCommander : public  Fighter, public std::enable_shared_from_this<SamuraiCommander>
{
public:

	SamuraiCommander();

	void idleState();

	void movement();

	void collision();

	void combat();

	void hitReaction();

	void hitReaction(String Enemy);

	void setAI(std::shared_ptr<Enemy> AI);

	float lerp(float start, float end, float t);

	bool Player_is_moving;

	bool Player_has_attacked;

	bool Player_is_blocking;

	int flip_idle_to_left;

	int combo_animation;

	Event this_event;

	Event my_second_event;

	Event my_third_event;

	Clock frameTimer;

	Clock buttonTimer;

	const float frameDuration = 16.7;

	float elapsedTime;

	float interpolationFactor;


private:

	std::shared_ptr<Aramusha> aramusha;

	std::shared_ptr<Enemy> AI;
};

