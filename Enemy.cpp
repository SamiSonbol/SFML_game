#include "Enemy.h"
#include "Aramusha.h"
#include "Fluids.h"
#include "SamuraiCommander.h"
#include <iostream>

Enemy::Enemy(){

	hurt_at_leftside.loadFromFile("Peasant_hurt_at_leftside.png");
	hurt_at_rightside.loadFromFile("Peasant_hurt_at_rightside.png");
	block_to_rightside.loadFromFile("Peasant_block_to_rightside.png");
	block_to_leftside.loadFromFile("Peasant_block_to_leftside.png");
	run_to_rightside.loadFromFile("Peasant_walk_to_rightside.png");
	run_to_leftside.loadFromFile("Peasant_walk_to_leftside.png");
	idle.loadFromFile("Idle_Peasant.png");
	idle_left.loadFromFile("Idle_Peasant_left.png");
	attack_from_rightside1.loadFromFile("Peasant_attack_from_rightside1.png");
	attack_from_leftside1.loadFromFile("Peasant_attack_from_leftside1.png");
	attack_from_rightside2.loadFromFile("Peasant_attack_from_rightside2.png");
	attack_from_leftside2.loadFromFile("Peasant_attack_from_leftside2.png");
	//attack_from_rightside3.loadFromFile("Peasant_attack_from_rightside3.png");
	//attack_from_leftside3.loadFromFile("Peasant_attack_from_leftside3.png");
	//dead_from_rightside.loadFromFile("Peasant_dead_from_rightside.png");
	//dead_from_leftside.loadFromFile("Peasant_dead_from_leftside.png");
	idle_sprite.setTexture(idle);
	idle_sprite.setTextureRect(IntRect(32, 25, 34, 71));
	idle_sprite.scale(6, 6);
	idle_sprite.setPosition(0, 580);

	clone_wound.loadFromFile("Idle_Peasant_wound.png");
	clone.setTexture(clone_wound);
	//idle_sprite.setTextureRect(IntRect(32, 25, 34, 71));
	//clone.setPosition(0, 580);
	//clone.setScale(6, 6);

	flip = 32;
	flip_toLeft = 705;
	flip_attack = 27;
	flip_attack_from_rightside = 317;
	flip_idle = 23;
	when_to_fallX = 25;
	when_to_fallY = 41;
	flip_block = 29;
	flip_block_to_leftside = 797;
	flip_dead_from_rightside = 690;
	flip_dead_from_leftside = 16;
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
	Player_pos = pos;
	my_event;
	stationary = true;
	time_clock;
	dt_clock;
	time = time_clock.restart().asSeconds();
	dt = dt_clock.restart().asSeconds();

}

void Enemy::setAramusha(std::shared_ptr<Aramusha> aramusha) {

	this->aramusha = aramusha;
}

void Enemy::setSamuraiCommander(std::shared_ptr<SamuraiCommander> commander) {

	this->commander = commander;
}

void Enemy::setFluids(std::shared_ptr<Fluids> blood_fluid) {

	this->blood_fluid = blood_fluid;
}

template <typename T>
void Enemy::idleState(std::shared_ptr<T> EnemyFighter) {

	    is_time_set = true;

		if (is_time_set) {

			time += time_clock.restart().asSeconds();

			if (time >= 3.f) {

				percent = 1 + (rand() % 100);

				time = 0.f;

				is_time_set = false;

			}

		}

		if (EnemyFighter->pos.x > pos.x) {

			Player_is_right_of_AI = true;
		}
		else {

			Player_is_right_of_AI = false;
		}

		if ((EnemyFighter->pos.x - pos.x >= 276 && Player_is_right_of_AI) || (pos.x - EnemyFighter->pos.x >= 276 && !Player_is_right_of_AI)) {

			AI_is_safe = true;

		}
		else {

			AI_is_safe = false;

		}

		if ((EnemyFighter->pos.x - pos.x <= 250 && Player_is_right_of_AI) || (pos.x - EnemyFighter->pos.x <= 250 && !Player_is_right_of_AI)) {

			combat_positions = true;

		}
		else {

			combat_positions = false;
		}

	pos = idle_sprite.getPosition();

	velocity.x = 0.f;
	velocity.y = 0.f;
	//------------------------------Enemy Idle-----------------------------------------------------		
	if ((!is_left) && stationary) {

		idle_sprite.setTexture(idle);

		if (flip_idle < 34) {

			flip_idle = 504;
		}

		idle_sprite.setTextureRect(IntRect(flip_idle, 28, 34, 68));

		flip_idle = flip_idle - 96;
	}

	if ((is_left) && stationary) {

		idle_sprite.setTexture(idle_left);

		if (flip_idle > 507) {

			flip_idle = 27;
		}

		idle_sprite.setTextureRect(IntRect(flip_idle, 28, 34, 68));

		flip_idle = flip_idle + 96;
	}

}

void Enemy::movement(int Aramusha) {

	//-------------------------------Enemy movement-------------------------------------------------
	if (Player_is_right_of_AI && !combat_positions && percent >= 50) {//moving to rightside

		stationary = false;

		is_left = false;

		velocity.x += movementSpeed * dt;

		idle_sprite.setTexture(run_to_rightside);

		if (flip > 705) {

			flip = 32;
		}

		idle_sprite.setTextureRect(IntRect(flip, 31, 37, 65));

		flip = flip + 96;
	}

	else if (!Player_is_right_of_AI && !combat_positions && percent >= 50) {//moving to leftside

		stationary = false;

		is_left = true;

		velocity.x += -movementSpeed * dt;

		idle_sprite.setTexture(run_to_leftside);

		if (flip_toLeft < 32) {

			flip_toLeft = 705;
		}

		idle_sprite.setTextureRect(IntRect(flip_toLeft, 31, 37, 65));
		
		flip_toLeft = flip_toLeft - 96;
	}

	else if (!Player_is_right_of_AI && percent <= 40 && !AI_is_safe && !is_attacking && !is_blocking) {//moving backwards to rightside

		stationary = false;

		is_left = false;

		velocity.x += movementSpeed * dt;

		idle_sprite.setTexture(run_to_leftside);

		if (flip > 705) {

			flip = 32;
		}

		idle_sprite.setTextureRect(IntRect(flip, 31, 37, 65));

		flip = flip + 96;

		is_left = true;
	}

	else if (Player_is_right_of_AI && percent <= 40 && !AI_is_safe && !is_attacking && !is_blocking) {//moving backwards to leftside

		stationary = false;

		is_left = true;

		velocity.x += -movementSpeed * dt;

		idle_sprite.setTexture(run_to_rightside);

		if (flip_toLeft < 32) {

			flip_toLeft = 705;
		}

		idle_sprite.setTextureRect(IntRect(flip_toLeft, 31, 37, 65));

		flip_toLeft = flip_toLeft - 96;

		is_left = false;
	}

	else {

		stationary = true;
	}

	idle_sprite.move(velocity);

		//for (int x = 0; x < blood_fluid->wounds.size(); x++) {

			//blood_fluid->wounds.at(x).move(velocity);

		//}

}

void Enemy::combat(int Aramusha) {

	//is_blocking = false;

	is_attacking = false;

	/**if (pos.x - aramusha->pos.x <= 420 && (percent < 10) && is_left) {//----attack1 from rightside

		idle_sprite.setTexture(attack_from_rightside1);

		if (flip_attack_from_rightside < 22) {

			flip_attack_from_rightside = 499;
		}

		idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 36, 50, 60));

		flip_attack_from_rightside = flip_attack_from_rightside - 95;
	}

	if (aramusha->pos.x - pos.x <= 420 && (percent < 10) && !is_left) {//----attack1 from leftside

		idle_sprite.setTexture(attack_from_leftside1);

		if (flip_attack > 499) {

			flip_attack = 22;
		}

		idle_sprite.setTextureRect(IntRect(flip_attack, 36, 50, 60));

		flip_attack = flip_attack + 95;
	}//-*/

	if ((percent <= 25) && is_left && combat_positions && !is_blocking) {//----attack2 from rightside

		idle_sprite.setTexture(attack_from_rightside2);

		if (flip_attack_from_rightside == 317) {

			idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 37, 39, 59));

			flip_attack_from_rightside = flip_attack_from_rightside - 56 - 43;

		}

		else if (flip_attack_from_rightside == 218) {

			idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 37, 43, 59));

			flip_attack_from_rightside = flip_attack_from_rightside - 54 - 64;

		} 

		else if (flip_attack_from_rightside == 100) {

			idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 37, 64, 59));

			flip_attack_from_rightside = flip_attack_from_rightside - 33 - 64;

		}

		else if (flip_attack_from_rightside == 3) {

			idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 37, 64, 59));

			flip_attack_from_rightside = 317;
		}

		is_attacking = true;
	}

	if ((percent <= 25) && !is_left && combat_positions && !is_blocking) {//----attack2 from leftside

		idle_sprite.setTexture(attack_from_leftside2);

	    if (flip_attack == 27) {

			idle_sprite.setTextureRect(IntRect(flip_attack, 37, 39, 59));

			flip_attack = flip_attack + 58 + 39;
		}

		else if (flip_attack == 124) {

			idle_sprite.setTextureRect(IntRect(flip_attack, 37, 43, 59));

			flip_attack = flip_attack + 54 + 43;
		}

		else if (flip_attack == 221) {

			idle_sprite.setTextureRect(IntRect(flip_attack, 37, 64, 59));

			flip_attack = flip_attack + 32 + 64;
		}

		else if (flip_attack == 317) {

			idle_sprite.setTextureRect(IntRect(flip_attack, 37, 64, 59));

			flip_attack = 27;

		}

		is_attacking = true;

	}//---*/

	//if ((((Player_pos.x) - (Enemy_pos.x) <= 300)) && ((percent / 100) >= 9.3) && (Enemy_is_left)) {//----attack3 from rightside
		//Enemy_idle_sprite.setTexture(Enemy_attack_from_rightside3);
		//if (Enemy_flip_attack_from_rightside < 3) {
		//	Enemy_flip_attack_from_rightside = 408;
		//}
		//Enemy_idle_sprite.setTextureRect(IntRect(Enemy_flip_attack_from_rightside, 37, 90, 93));
		//sleep(milliseconds(10));
		//Enemy_flip_attack_from_rightside = Enemy_flip_attack_from_rightside - 132;
	//}

	//if (((Player_pos.x) - (Enemy_pos.x) <= 300) && ((percent / 100) >= 9.3) && !(Enemy_is_left)) {//----attack3 from leftside
		//Enemy_idle_sprite.setTexture(Enemy_attack_from_leftside3);
		//if (Enemy_flip_attack > 407) {
		//	Enemy_flip_attack = 27;
		//}
		//Enemy_idle_sprite.setTextureRect(IntRect(Enemy_flip_attack, 37, 90, 93));
		//sleep(milliseconds(15));
		//Enemy_flip_attack = Enemy_flip_attack + 127;
	//}//-

	if ((aramusha->Player_has_attacked) && (percent <= 100) && !is_left && !AI_is_safe && !is_attacking) {//--block to rightside

		idle_sprite.setTexture(block_to_rightside);

		is_blocking = true;

			if (flip_block == 29) {

				idle_sprite.setPosition(pos.x, 610);

				idle_sprite.setTextureRect(IntRect(flip_block, 38, 37, 58));

				flip_block = flip_block + 61 + 37;

			}

			else if (flip_block == 127) {

				idle_sprite.setPosition(pos.x, 660);

				idle_sprite.setTextureRect(IntRect(flip_block, 44, 34, 52));

				flip_block = flip_block + 62 + 34;

			}

			else if (flip_block == 223) {

				idle_sprite.setPosition(pos.x, 710);

				idle_sprite.setTextureRect(IntRect(flip_block, 54, 34, 42));

				flip_block = flip_block + 63 + 34;

			}

			else if (flip_block == 320) {

				idle_sprite.setPosition(pos.x, 760);

				idle_sprite.setTextureRect(IntRect(flip_block, 59, 34, 37));

				flip_block = flip_block + 62 + 34;

			}

			else if (flip_block == 416) {

		        idle_sprite.setPosition(pos.x, 800);

				idle_sprite.setTextureRect(IntRect(flip_block, 63, 34, 33));

				flip_block = flip_block + 62 + 34;

			}

			else if (flip_block == 512) {

				idle_sprite.setPosition(pos.x, 760);

				idle_sprite.setTextureRect(IntRect(flip_block, 59, 34, 37));

				flip_block = flip_block + 61 + 34;

			}

			else if (flip_block == 607) {

				idle_sprite.setPosition(pos.x, 710);

				idle_sprite.setTextureRect(IntRect(flip_block, 54, 34, 42));

				flip_block = flip_block + 62 + 34;

			}

			else if (flip_block == 703) {

				idle_sprite.setPosition(pos.x, 660);

				idle_sprite.setTextureRect(IntRect(flip_block, 44, 34, 52));

				flip_block = flip_block + 60 + 34;

			}

			else if (flip_block == 797) {

				idle_sprite.setPosition(pos.x, 580);

				idle_sprite.setTextureRect(IntRect(flip_block, 38, 37, 58));

				flip_block = 29;

				aramusha->Player_has_attacked = false;

				is_blocking = false;

			}

	}

	if ((aramusha->Player_has_attacked) && (percent <= 100) && is_left && !AI_is_safe && !is_attacking) {//--block to leftside

		idle_sprite.setTexture(block_to_leftside);

		is_blocking = true;

		if (flip_block_to_leftside == 797) {

			idle_sprite.setPosition(pos.x, 610);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 38, 37, 58));

			flip_block_to_leftside = 703;

		}

		else if (flip_block_to_leftside == 703) {

			idle_sprite.setPosition(pos.x, 660);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 44, 34, 52));

			flip_block_to_leftside = 607;

		}

		else if (flip_block_to_leftside == 607) {

			idle_sprite.setPosition(pos.x, 710);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 54, 34, 42));

			flip_block_to_leftside = 512;

		}

		else if (flip_block_to_leftside == 512) {

			idle_sprite.setPosition(pos.x, 760);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 59, 34, 37));

			flip_block_to_leftside = 416;

		}

		else if (flip_block == 416) {

			idle_sprite.setPosition(pos.x, 800);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 63, 34, 33));

			flip_block_to_leftside = 320;

		}

		else if (flip_block_to_leftside == 320) {

			idle_sprite.setPosition(pos.x, 760);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 59, 34, 37));

			flip_block_to_leftside = 223;

		}

		else if (flip_block_to_leftside == 223) {

			idle_sprite.setPosition(pos.x, 710);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 54, 34, 42));

			flip_block_to_leftside = 127;

		}

		else if (flip_block_to_leftside == 127) {

			idle_sprite.setPosition(pos.x, 660);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 44, 34, 52));

			flip_block_to_leftside = 29;

		}

	    else if (flip_block_to_leftside == 29) {

		    idle_sprite.setPosition(pos.x, 580);

		    idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 38, 37, 58));

		    flip_block_to_leftside = 797;

		    aramusha->Player_has_attacked = false;

			is_blocking = false;

	    }


	}


	//if (idle_sprite.getGlobalBounds().intersects(aramusha->idle_sprite.getGlobalBounds()) + 37 && percent == 40) {

		

	//}*/

}

void Enemy::movement() {

	//-------------------------------Enemy movement-------------------------------------------------
	if (Player_is_right_of_AI && !combat_positions && percent >= 50) {//moving to rightside

		stationary = false;

		is_left = false;

		velocity.x += movementSpeed * dt;

		idle_sprite.setTexture(run_to_rightside);

		if (flip > 705) {

			flip = 32;
		}

		idle_sprite.setTextureRect(IntRect(flip, 31, 37, 65));

		flip = flip + 96;
	}

	else if (!Player_is_right_of_AI && !combat_positions && percent >= 50) {//moving to leftside

		stationary = false;

		is_left = true;

		velocity.x += -movementSpeed * dt;

		idle_sprite.setTexture(run_to_leftside);

		if (flip_toLeft < 32) {

			flip_toLeft = 705;
		}

		idle_sprite.setTextureRect(IntRect(flip_toLeft, 31, 37, 65));

		flip_toLeft = flip_toLeft - 96;
	}

	else if (!Player_is_right_of_AI && percent <= 40 && !AI_is_safe && !is_attacking && !is_blocking) {//moving backwards to rightside

		stationary = false;

		is_left = false;

		velocity.x += movementSpeed * dt;

		idle_sprite.setTexture(run_to_leftside);

		if (flip > 705) {

			flip = 32;
		}

		idle_sprite.setTextureRect(IntRect(flip, 31, 37, 65));

		flip = flip + 96;

		is_left = true;
	}

	else if (Player_is_right_of_AI && percent <= 40 && !AI_is_safe && !is_attacking && !is_blocking) {//moving backwards to leftside

		stationary = false;

		is_left = true;

		velocity.x += -movementSpeed * dt;

		idle_sprite.setTexture(run_to_rightside);

		if (flip_toLeft < 32) {

			flip_toLeft = 705;
		}

		idle_sprite.setTextureRect(IntRect(flip_toLeft, 31, 37, 65));

		flip_toLeft = flip_toLeft - 96;

		is_left = false;
	}

	else {

		stationary = true;
	}

	idle_sprite.move(velocity);

}

void Enemy::combat() {

	//is_blocking = false;

	is_attacking = false;

	/**if (pos.x - commander->pos.x <= 420 && (percent < 10) && is_left) {//----attack1 from rightside

		idle_sprite.setTexture(attack_from_rightside1);

		if (flip_attack_from_rightside < 22) {

			flip_attack_from_rightside = 499;
		}

		idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 36, 50, 60));

		flip_attack_from_rightside = flip_attack_from_rightside - 95;
	}

	if (commander->pos.x - pos.x <= 420 && (percent < 10) && !is_left) {//----attack1 from leftside

		idle_sprite.setTexture(attack_from_leftside1);

		if (flip_attack > 499) {

			flip_attack = 22;
		}

		idle_sprite.setTextureRect(IntRect(flip_attack, 36, 50, 60));

		flip_attack = flip_attack + 95;
	}//-*/

	if ((percent <= 40) && is_left && combat_positions && !is_blocking) {//----attack2 from rightside

		idle_sprite.setTexture(attack_from_rightside2);

		if (flip_attack_from_rightside == 317) {

			idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 37, 39, 59));

			flip_attack_from_rightside = flip_attack_from_rightside - 56 - 43;

		}

		else if (flip_attack_from_rightside == 218) {

			idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 37, 43, 59));

			flip_attack_from_rightside = flip_attack_from_rightside - 54 - 64;

		}

		else if (flip_attack_from_rightside == 100) {

			idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 37, 64, 59));

			flip_attack_from_rightside = flip_attack_from_rightside - 33 - 64;

		}

		else if (flip_attack_from_rightside == 3) {

			idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 37, 64, 59));

			flip_attack_from_rightside = 317;
		}

		is_attacking = true;
	}

	if ((percent <= 40) && !is_left && combat_positions && !is_blocking) {//----attack2 from leftside

		idle_sprite.setTexture(attack_from_leftside2);

		if (flip_attack == 27) {

			idle_sprite.setTextureRect(IntRect(flip_attack, 37, 39, 59));

			flip_attack = flip_attack + 58 + 39;
		}

		else if (flip_attack == 124) {

			idle_sprite.setTextureRect(IntRect(flip_attack, 37, 43, 59));

			flip_attack = flip_attack + 54 + 43;
		}

		else if (flip_attack == 221) {

			idle_sprite.setTextureRect(IntRect(flip_attack, 37, 64, 59));

			flip_attack = flip_attack + 32 + 64;
		}

		else if (flip_attack == 317) {

			idle_sprite.setTextureRect(IntRect(flip_attack, 37, 64, 59));

			flip_attack = 27;

		}

		is_attacking = true;

	}//---*/

	//if ((((Player_pos.x) - (Enemy_pos.x) <= 300)) && ((percent / 100) >= 9.3) && (Enemy_is_left)) {//----attack3 from rightside
		//Enemy_idle_sprite.setTexture(Enemy_attack_from_rightside3);
		//if (Enemy_flip_attack_from_rightside < 3) {
		//	Enemy_flip_attack_from_rightside = 408;
		//}
		//Enemy_idle_sprite.setTextureRect(IntRect(Enemy_flip_attack_from_rightside, 37, 90, 93));
		//sleep(milliseconds(10));
		//Enemy_flip_attack_from_rightside = Enemy_flip_attack_from_rightside - 132;
	//}

	//if (((Player_pos.x) - (Enemy_pos.x) <= 300) && ((percent / 100) >= 9.3) && !(Enemy_is_left)) {//----attack3 from leftside
		//Enemy_idle_sprite.setTexture(Enemy_attack_from_leftside3);
		//if (Enemy_flip_attack > 407) {
		//	Enemy_flip_attack = 27;
		//}
		//Enemy_idle_sprite.setTextureRect(IntRect(Enemy_flip_attack, 37, 90, 93));
		//sleep(milliseconds(15));
		//Enemy_flip_attack = Enemy_flip_attack + 127;
	//}//-

	if ((commander->Player_has_attacked) && (percent <= 100) && !is_left && !AI_is_safe && !is_attacking) {//--block to rightside

		idle_sprite.setTexture(block_to_rightside);

		is_blocking = true;

		if (flip_block == 29) {

			idle_sprite.setPosition(pos.x, 610);

			idle_sprite.setTextureRect(IntRect(flip_block, 38, 37, 58));

			flip_block = flip_block + 61 + 37;

		}

		else if (flip_block == 127) {

			idle_sprite.setPosition(pos.x, 660);

			idle_sprite.setTextureRect(IntRect(flip_block, 44, 34, 52));

			flip_block = flip_block + 62 + 34;

		}

		else if (flip_block == 223) {

			idle_sprite.setPosition(pos.x, 710);

			idle_sprite.setTextureRect(IntRect(flip_block, 54, 34, 42));

			flip_block = flip_block + 63 + 34;

		}

		else if (flip_block == 320) {

			idle_sprite.setPosition(pos.x, 760);

			idle_sprite.setTextureRect(IntRect(flip_block, 59, 34, 37));

			flip_block = flip_block + 62 + 34;

		}

		else if (flip_block == 416) {

			idle_sprite.setPosition(pos.x, 800);

			idle_sprite.setTextureRect(IntRect(flip_block, 63, 34, 33));

			flip_block = flip_block + 62 + 34;

		}

		else if (flip_block == 512) {

			idle_sprite.setPosition(pos.x, 760);

			idle_sprite.setTextureRect(IntRect(flip_block, 59, 34, 37));

			flip_block = flip_block + 61 + 34;

		}

		else if (flip_block == 607) {

			idle_sprite.setPosition(pos.x, 710);

			idle_sprite.setTextureRect(IntRect(flip_block, 54, 34, 42));

			flip_block = flip_block + 62 + 34;

		}

		else if (flip_block == 703) {

			idle_sprite.setPosition(pos.x, 660);

			idle_sprite.setTextureRect(IntRect(flip_block, 44, 34, 52));

			flip_block = flip_block + 60 + 34;

		}

		else if (flip_block == 797) {

			idle_sprite.setPosition(pos.x, 580);

			idle_sprite.setTextureRect(IntRect(flip_block, 38, 37, 58));

			flip_block = 29;

			commander->Player_has_attacked = false;

			is_blocking = false;

		}

	}

	if ((commander->Player_has_attacked) && (percent <= 100) && is_left && !AI_is_safe && !is_attacking) {//--block to leftside

		idle_sprite.setTexture(block_to_leftside);

		is_blocking = true;

		if (flip_block_to_leftside == 797) {

			idle_sprite.setPosition(pos.x, 610);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 38, 37, 58));

			flip_block_to_leftside = 703;

		}

		else if (flip_block_to_leftside == 703) {

			idle_sprite.setPosition(pos.x, 660);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 44, 34, 52));

			flip_block_to_leftside = 607;

		}

		else if (flip_block_to_leftside == 607) {

			idle_sprite.setPosition(pos.x, 710);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 54, 34, 42));

			flip_block_to_leftside = 512;

		}

		else if (flip_block_to_leftside == 512) {

			idle_sprite.setPosition(pos.x, 760);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 59, 34, 37));

			flip_block_to_leftside = 416;

		}

		else if (flip_block == 416) {

			idle_sprite.setPosition(pos.x, 800);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 63, 34, 33));

			flip_block_to_leftside = 320;

		}

		else if (flip_block_to_leftside == 320) {

			idle_sprite.setPosition(pos.x, 760);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 59, 34, 37));

			flip_block_to_leftside = 223;

		}

		else if (flip_block_to_leftside == 223) {

			idle_sprite.setPosition(pos.x, 710);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 54, 34, 42));

			flip_block_to_leftside = 127;

		}

		else if (flip_block_to_leftside == 127) {

			idle_sprite.setPosition(pos.x, 660);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 44, 34, 52));

			flip_block_to_leftside = 29;

		}

		else if (flip_block_to_leftside == 29) {

			idle_sprite.setPosition(pos.x, 580);

			idle_sprite.setTextureRect(IntRect(flip_block_to_leftside, 38, 37, 58));

			flip_block_to_leftside = 797;

			commander->Player_has_attacked = false;

			is_blocking = false;

		}
		
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

void Enemy::live(int Aramusha) {

	idleState(aramusha);

	movement(1);

	screen_collision();

	combat(1);

	fighter_collision(aramusha);

}