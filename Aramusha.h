#pragma once
#include "Fighter.h"
#include <functional>
#include <memory>
#include <SFML/Graphics.hpp>

using namespace sf;
//
//class SamuraiCommander;
//
//class Enemy;
//
//class Physics;

class Aramusha : public Fighter
{

public:

	Spritesheet jump_to_rightside = Spritesheet("resources\\Aramusha\\Aramusha_jump_to_rightside.png", {2, 1});
	Spritesheet jump_to_leftside = Spritesheet("resources\\Aramusha\\Aramusha_jump_to_leftside.png", { 2, 1 });
	Spritesheet idle_looking_at_left_side = Spritesheet("resources\\Aramusha\\Aramusha_idle_looking_AtLeftside.png", { 8, 1 });
	Spritesheet run_to_rightside = Spritesheet("resources\\Aramusha\\Aramusha_run_to_rightside.png", { 8, 1 });
	Spritesheet run_to_leftside = Spritesheet("resources\\Aramusha\\Aramusha_run_to_leftside.png", { 8, 1 });
	Spritesheet idle = Spritesheet("resources\\Aramusha\\Idle_Aramusha.png", { 8, 1 });
	Spritesheet idle_left = Spritesheet("resources\\Aramusha\\Idle_Aramusha_left.png", { 8, 1 });
	Spritesheet attack_from_rightside1 = Spritesheet("resources\\Aramusha\\Aramusha_attack_from_rightside.png", { 6, 1 });
	Spritesheet attack_from_leftside1 = Spritesheet("resources\\Aramusha\\Aramusha_attack_from_leftside.png", { 6, 1 });

	Aramusha();

	

	void idleState(const float& delta_time);
	void movement(const float& delta_time);
	void combat(const float& delta_time);
	void live(Fighter& enemy_fighter, const float& delta_time);

	Event this_event;

	Image bloodyAramusha;


private: 

	//std::shared_ptr<SamuraiCommander> commander;

	//std::shared_ptr<Enemy> AI;

	sf::Vector2f sprite_scale;

};


