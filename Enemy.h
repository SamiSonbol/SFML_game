#pragma once
#include "Fighter.h"
#include <memory>
#include <SFML/Graphics.hpp>

using namespace sf;

class Aramusha;

class SamuraiCommander;

class Fluids;

class Enemy : public Fighter, public std::enable_shared_from_this<Enemy>
{

public:

	Enemy();

	void movement();

	void combat();

	void movement(int Aramusha);

	void combat(int Aramusha);

	void setAramusha(std::shared_ptr<Aramusha> aramusha);

	void setSamuraiCommander(std::shared_ptr<SamuraiCommander> commander);

	void setFluids(std::shared_ptr<Fluids> blood_fluid);

	void live(int Aramusha);

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

	template <typename T>
	void idleState(std::shared_ptr<T> EnemyFighter);

private:  
	
	std::shared_ptr<Aramusha> aramusha;

	std::shared_ptr<SamuraiCommander> commander;

	std::shared_ptr<Fluids> blood_fluid;

};

