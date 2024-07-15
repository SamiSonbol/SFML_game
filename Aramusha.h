#pragma once
#include "Fighter.h"
#include <functional>
#include <memory>
#include <SFML/Graphics.hpp>

using namespace sf;

class SamuraiCommander;

class Enemy;

class Physics;

class Aramusha : public Fighter, public std::enable_shared_from_this<Aramusha>
{

public:

	Aramusha();

	void idleState();

	void movement();

	void combat();
	
	void setSamuraiCommander(std::shared_ptr<SamuraiCommander> commander);

	void setEnemy(std::shared_ptr<Enemy> AI);

	void live();

	Event this_event;

	Image bloodyAramusha;


private: 

	std::shared_ptr<SamuraiCommander> commander;

	std::shared_ptr<Enemy> AI;

};


