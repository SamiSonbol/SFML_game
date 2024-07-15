#include "Aramusha.h"
#include "Enemy.h"
#include "SamuraiCommander.h"
#include <iostream>
#include <typeinfo>

Aramusha::Aramusha(){

	jump_to_rightside.loadFromFile("Aramusha_jump_to_rightside.png");
	jump_to_leftside.loadFromFile("Aramusha_jump_to_leftside.png");
	idle_looking_AtLeftside.loadFromFile("Aramusha_idle_looking_AtLeftside.png");
	run_to_rightside.loadFromFile("Aramusha_run_to_rightside.png");
	run_to_leftside.loadFromFile("Aramusha_run_to_leftside.png");
	idle.loadFromFile("Idle_Aramusha.png");
	idle_left.loadFromFile("Idle_Aramusha_left.png");
	attack_from_rightside1.loadFromFile("Aramusha_attack_from_rightside.png");
	attack_from_leftside1.loadFromFile("Aramusha_attack_from_leftside.png");
	idle_sprite.setTexture(idle);
	idle_sprite.setTextureRect(IntRect(73, 68, 44, 50));
	idle_sprite.scale(8, 8);
	idle_sprite.setPosition(1920, 575);
	
	flip = 73;
	flip_toLeft;
	flip_attack = 27;
	flip_attack_from_rightside = 408;
	flip_idle = 73;
	when_to_fallX = 75;
	when_to_fallY = 67;
	window_height = 1080;
	window_width = 1920;
	movementSpeed = 600.f;

	is_left = true;
	is_on_ground = false;
	already_picked = false;
	was_chosen = false;
	stationary = false;
	Player_is_on_rightside = false;
	Player_is_attacking = false;
	Player_is_moving = false;
	Player_has_attacked = false;

	my_event.type = Event::KeyReleased;
	my_event.key.code = Keyboard::Space;
	
	dt = dt_clock.restart().asSeconds();

	bloodyAramusha = idle_left.copyToImage();
	
}

void Aramusha::setSamuraiCommander(std::shared_ptr<SamuraiCommander> commander) {

	this->commander = commander;
}

void Aramusha::setEnemy(std::shared_ptr<Enemy> AI) {

	this->AI = AI;
}


void Aramusha::idleState() {

	Player_is_moving = false;

	pos = idle_sprite.getPosition();

	Player_pos = pos;

	velocity.x = 0.f;
	velocity.y = 0.f;

	if (!is_left) {

		idle_sprite.setTexture(idle);

		if (flip_idle > 1473) {

			flip_idle = 73;
		}

		idle_sprite.setTextureRect(IntRect(flip_idle, 68, 44, 50));

		flip_idle = flip_idle + 200;

		Player_is_on_rightside = false;
		
	}

	if (is_left) {

		idle_sprite.setTexture(idle_looking_AtLeftside);

		if (flip_idle > 1473) {

			flip_idle = 73;
		}

		idle_sprite.setTextureRect(IntRect(flip_idle, 68, 44, 50));

		flip_idle = flip_idle + 200;

		Player_is_on_rightside = true;
	}
}

void Aramusha::movement() {
	
	//-------------------------------Aramusha movement-------------------------------------------------
	if (Keyboard::isKeyPressed(Keyboard::Right) && !stationary) {//moving to rightside

		is_left = false;

		velocity.x += movementSpeed * dt;

		idle_sprite.setTexture(run_to_rightside);

		if (flip > 1473) {

			flip = 73;
		}

		idle_sprite.setTextureRect(IntRect(flip, 68, 44, 50));

		//sleep(milliseconds(0.2));

		flip = flip + 200;

		Player_is_on_rightside = true;

		Player_is_moving = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Left) && !stationary) {//moving to leftside

		is_left = true;

		velocity.x += -movementSpeed * dt;

		idle_sprite.setTexture(run_to_leftside);

		if (flip > 1473) {

			flip = 73;

		}

		idle_sprite.setTextureRect(IntRect(flip, 68, 44, 50));

		//sleep(milliseconds(0.2));

		flip = flip + 200;

		Player_is_on_rightside = false;

		Player_is_moving = true;
	}

	idle_sprite.move(velocity);

}

void Aramusha::combat() {

	//--------------------------Aramusha Combat---------------------------------------
	if (Keyboard::isKeyPressed(Keyboard::Space)) {

		is_attacking = false;

		Player_has_attacked = true;

	}

	if (my_event.type && my_event.key.code && !is_attacking && is_left) {//----attack1 from rightside

		Player_is_attacking = true;

		idle_sprite.setTexture(attack_from_rightside1);

		if (flip_attack_from_rightside < 48) {

			flip_attack_from_rightside = 1063;

			is_attacking = true;

		    Player_is_attacking = false;

		}

		idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 68, 80, 50));

		flip_attack_from_rightside = flip_attack_from_rightside - 200;

	}

	if (my_event.type && my_event.key.code && !is_attacking && !is_left) {//----attack1 from leftside

		Player_is_attacking = true;

		idle_sprite.setTexture(attack_from_leftside1);

		if (flip_attack > 1063) {

			flip_attack = 48;

			is_attacking = true;

			Player_is_attacking = false;

		}

		idle_sprite.setTextureRect(IntRect(flip_attack, 68, 80, 50));

		flip_attack = flip_attack + 200;

	}

}

template <typename U>
void Fighter::fighter_collision(std::shared_ptr<U> EnemyFighter) {

	if (idle_sprite.getGlobalBounds().intersects(EnemyFighter->idle_sprite.getGlobalBounds()) && (Keyboard::isKeyPressed(Keyboard::Right))) {

		idle_sprite.setPosition(pos);

		EnemyFighter->idle_sprite.setPosition(EnemyFighter->pos);

	}

	else if (idle_sprite.getGlobalBounds().intersects(EnemyFighter->idle_sprite.getGlobalBounds()) && (Keyboard::isKeyPressed(Keyboard::Left))) {

		idle_sprite.setPosition(pos);

		EnemyFighter->idle_sprite.setPosition(EnemyFighter->pos);

	}

	else if (idle_sprite.getGlobalBounds().intersects(EnemyFighter->idle_sprite.getGlobalBounds())) {

		idle_sprite.setPosition(pos);

		EnemyFighter->idle_sprite.setPosition(EnemyFighter->pos);

	}

}

void Aramusha::live() {

	idleState();

	movement();

	screen_collision();

	fighter_collision(AI);

	combat();

}