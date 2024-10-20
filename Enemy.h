#pragma once
#include "Fighter.h"
#include "Aramusha.h"
#include <memory>
#include <SFML/Graphics.hpp>

using namespace sf;

//class Aramusha;
//
//class SamuraiCommander;
//
//class Fluids;

class Enemy : public Fighter
{

public:

	Spritesheet hurt_at_leftside = Spritesheet("resources\\Peasant\\Peasant_hurt_at_leftside.png", { 2, 1 });
	Spritesheet hurt_at_rightside = Spritesheet("resources\\Peasant\\Peasant_hurt_at_rightside.png", { 2, 1 });
	Spritesheet block_to_rightside = Spritesheet("resources\\Peasant\\Peasant_block_to_rightside.png", { 9, 1 });
	Spritesheet block_to_leftside = Spritesheet("resources\\Peasant\\Peasant_block_to_leftside.png", { 9, 1 });
	Spritesheet run_to_rightside = Spritesheet("resources\\Peasant\\Peasant_walk_to_rightside.png", { 8, 1 });
	Spritesheet run_to_leftside = Spritesheet("resources\\Peasant\\Peasant_walk_to_leftside.png", { 8, 1 });
	Spritesheet idle = Spritesheet("resources\\Peasant\\Idle_Peasant.png", { 6, 1 });
	Spritesheet idle_left = Spritesheet("resources\\Peasant\\Idle_Peasant_left.png", { 6, 1 });
	Spritesheet attack_from_rightside1 = Spritesheet("resources\\Peasant\\Peasant_attack_from_rightside1.png", { 6, 1 });
	Spritesheet attack_from_leftside1 = Spritesheet("resources\\Peasant\\Peasant_attack_from_leftside1.png", { 6, 1 });
	Spritesheet attack_from_rightside2 = Spritesheet("resources\\Peasant\\Peasant_attack_from_rightside2.png", { 4, 1 });
	Spritesheet attack_from_leftside2 = Spritesheet("resources\\Peasant\\Peasant_attack_from_leftside2.png", { 4, 1 });

	Enemy();

	void movement(const float& delta_time);

	void combat(const float& delta_time);

	/*void setAramusha(std::shared_ptr<Aramusha> aramusha);

	void setSamuraiCommander(std::shared_ptr<SamuraiCommander> commander);

	void setFluids(std::shared_ptr<Fluids> blood_fluid);*/
	

	bool stationary;

	int percent;

	int attack_width;

	int flip_block_to_leftside;

	bool is_time_set;

	bool Player_is_right_of_AI;

	bool AI_is_safe;

	bool combat_positions;

	float time;

	Clock time_clock;

	Sprite clone;

	Texture clone_wound;

	

	void idleState(Fighter& enemy_fighter, const float& delta_time);
	void movement(Fighter& enemy_fighter, const float& delta_time);
	void combat(Fighter& enemy_fighter, const float& delta_time);
	void live(Fighter& enemy_fighter, const float& delta_time);

private:  

	//std::shared_ptr<Fluids> blood_fluid;

	sf::Vector2f sprite_scale;

};

