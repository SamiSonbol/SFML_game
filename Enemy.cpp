#include "Enemy.h"
#include <iostream>

Enemy::Enemy(){

	
	//attack_from_rightside3.loadFromFile("Peasant_attack_from_rightside3.png");
	//attack_from_leftside3.loadFromFile("Peasant_attack_from_leftside3.png");
	//dead_from_rightside.loadFromFile("Peasant_dead_from_rightside.png");
	//dead_from_leftside.loadFromFile("Peasant_dead_from_leftside.png");
	this->sprite_scale = { 4, 4 };
	update_sprite(this->idle, this->main_sprite, this->sprite_scale, 1);
	
	this->main_sprite.setPosition(0, 580);

	clone_wound.loadFromFile("resources\\Peasant\\Idle_Peasant_wound.png");
	clone.setTexture(clone_wound);
	//main_sprite.setTextureRect(IntRect(32, 25, 34, 71));
	//clone.setPosition(0, 580);
	//clone.setScale(6, 6);
	
	attack_width = 39;
	window_height = 1080;
	window_width = 1920;
	percent = 1 + (rand() % 100);
	movementSpeed = 600.f;
		
	is_left = false;
	is_on_ground = false;
	is_blocking = false;
	already_picked = false;
	was_chosen = false;
	dead = false;
	is_attacking = false;
	is_time_set = true;
	Player_is_right_of_AI = false;
	AI_is_safe = false;
	combat_positions = false;

	velocity;
	my_event;
	stationary = true;
	time_clock;
	time = time_clock.restart().asSeconds();

}

//void Enemy::setAramusha(std::shared_ptr<Aramusha> aramusha) {
//
//	this->aramusha = aramusha;
//}
//
//void Enemy::setSamuraiCommander(std::shared_ptr<SamuraiCommander> commander) {
//
//	this->commander = commander;
//}
//
//void Enemy::setFluids(std::shared_ptr<Fluids> blood_fluid) {
//
//	this->blood_fluid = blood_fluid;
//}

void Enemy::idleState(Fighter& enemy_fighter, const float& delta_time) {

	is_time_set = true;

	if (is_time_set) {

		time += time_clock.restart().asSeconds();

		if (time >= 3.f) {

			percent = 1 + (rand() % 100);

			time = 0.f;

			is_time_set = false;

		}

	};

	/*if (!enemy_fighter.intersects(this->main_sprite)) {

		AI_is_safe = true;

	}
	else {

		AI_is_safe = false;

	};*/

	velocity.x = 0.0f;
	velocity.y = 0.0f;
	//------------------------------Enemy Idle-----------------------------------------------------		
	if (this->is_on_right_side(enemy_fighter.main_sprite) && stationary) {

		update_sprite(this->idle_left, this->main_sprite, this->sprite_scale, delta_time);

	};

	if (this->is_on_left_side(enemy_fighter.main_sprite) && stationary) {

		update_sprite(this->idle, this->main_sprite, this->sprite_scale, delta_time);

	};

};

void Enemy::movement(Fighter& enemy_fighter, const float& delta_time) {

	sf::Vector2f pos = this->main_sprite.getPosition();

	if (enemy_fighter.in_range(255, pos)) {

		this->combat_positions = true;
		this->AI_is_safe = false;

	}
	else {

		this->combat_positions = false;
		this->AI_is_safe = true;

	};

	if (enemy_fighter.is_on_right_side(this->main_sprite) && !combat_positions && percent >= 50 && !this->is_attacking && !this->is_blocking) {//moving to rightside

		stationary = false;

		velocity.x += movementSpeed * delta_time;

		update_sprite(this->run_to_rightside, this->main_sprite, this->sprite_scale, delta_time);

	}
	else if (enemy_fighter.is_on_left_side(this->main_sprite) && !combat_positions && percent >= 50 && !this->is_attacking && !this->is_blocking) {//moving to leftside

		stationary = false;

		velocity.x += -movementSpeed * delta_time;

		update_sprite(this->run_to_leftside, this->main_sprite, this->sprite_scale, delta_time);

	}
	else if (enemy_fighter.is_on_left_side(this->main_sprite) && percent <= 40 && !AI_is_safe && !this->is_attacking && !this->is_blocking) {//moving backwards to rightside

		stationary = false;

		velocity.x += movementSpeed * delta_time;

		update_sprite(this->run_to_leftside, this->main_sprite, this->sprite_scale, delta_time);

	}
	else if (enemy_fighter.is_on_right_side(this->main_sprite) && percent <= 40 && !this->AI_is_safe && !this->is_attacking && !this->is_blocking) {//moving backwards to leftside

		stationary = false;

		velocity.x += -movementSpeed * delta_time;

		update_sprite(this->run_to_rightside, this->main_sprite, this->sprite_scale, delta_time);

	}
	else {

		stationary = true;

	};

	main_sprite.move(velocity);

	//for (int x = 0; x < blood_fluid->wounds.size(); x++) {

		//blood_fluid->wounds.at(x).move(velocity);

	//}

};

void Enemy::combat(Fighter& enemy_fighter, const float& delta_time) {

	sf::Vector2f pos = this->main_sprite.getPosition();

	//is_blocking = false;

	is_attacking = false;

	if ((percent <= 25) && enemy_fighter.is_on_left_side(this->main_sprite) && combat_positions && !this->is_blocking) {//----attack2 from rightside

		update_sprite(this->attack_from_rightside2, this->main_sprite, this->sprite_scale, delta_time);

		is_attacking = true;
	};

	if ((percent <= 25) && enemy_fighter.is_on_right_side(this->main_sprite) && combat_positions && !this->is_blocking) {//----attack2 from leftside

		update_sprite(this->attack_from_leftside2, this->main_sprite, this->sprite_scale, delta_time);

		is_attacking = true;

	};

	if (enemy_fighter.Player_has_attacked && (percent <= 100) && enemy_fighter.is_on_right_side(this->main_sprite) && !this->AI_is_safe && !this->is_attacking) {//--block to rightside

		is_blocking = true;

		update_sprite(this->block_to_rightside, this->main_sprite, this->sprite_scale, delta_time);

		enemy_fighter.Player_has_attacked = false;

		is_blocking = false;

	};

	if (enemy_fighter.Player_has_attacked && (percent <= 100) && enemy_fighter.is_on_left_side(this->main_sprite) && !this->AI_is_safe && !this->is_attacking) {//--block to leftside

		is_blocking = true;

		update_sprite(this->block_to_leftside, this->main_sprite, this->sprite_scale, delta_time);

		enemy_fighter.Player_has_attacked = false;

		is_blocking = false;

	};

	//if (main_sprite.getGlobalBounds().intersects(aramusha->main_sprite.getGlobalBounds()) + 37 && percent == 40) {

		

	//}*/

};

void Enemy::movement(const float& delta_time) {
	
}

void Enemy::combat(const float& delta_time) {

};


void Enemy::live(Fighter& enemy_fighter, const float& delta_time) {

	idleState(enemy_fighter, delta_time);

	movement(enemy_fighter, delta_time);

	//screen_collision();

	combat(enemy_fighter, delta_time);

	//fighter_collision(enemy_fighter);

};

