#include "SamuraiCommander.h"
#include "Aramusha.h"
#include "Enemy.h"
#include <iostream>

    SamuraiCommander::SamuraiCommander(){

	hurt_at_leftside.loadFromFile("SamuraiCommander_hurt_at_leftside.png");
	hurt_at_rightside.loadFromFile("SamuraiCommander_hurt_at_rightside.png");
	block_to_rightside.loadFromFile("SamuraiCommander_block_to_rightside.png");
	block_to_leftside.loadFromFile("SamuraiCommander_block_to_leftside.png");
	run_to_rightside.loadFromFile("SamuraiCommander_run_to_rightside.png");
	run_to_leftside.loadFromFile("SamuraiCommander_run_to_leftside.png");
	idle.loadFromFile("Idle_SamuraiCommander.png");
	idle_left.loadFromFile("Idle_SamuraiCommander_left.png");
	attack_from_rightside1.loadFromFile("SamuraiCommander_attack_from_rightside1.png");
	attack_from_leftside1.loadFromFile("SamuraiCommander_attack_from_leftside1.png");
	attack_from_rightside2.loadFromFile("SamuraiCommander_attack_from_rightside2.png");
	attack_from_leftside2.loadFromFile("SamuraiCommander_attack_from_leftside2.png");
	attack_from_rightside3.loadFromFile("SamuraiCommander_attack_from_rightside3.png");
	attack_from_leftside3.loadFromFile("SamuraiCommander_attack_from_leftside3.png");
	dead_from_rightside.loadFromFile("SamuraiCommander_dead_from_rightside.png");
	dead_from_leftside.loadFromFile("SamuraiCommander_dead_from_leftside.png");
	idle_sprite.setTexture(idle);
	idle_sprite.setTextureRect(IntRect(73, 68, 44, 50));
	idle_sprite.scale(6, 6);
	idle_sprite.setPosition(1080, 425);

	flip = 10;
	flip_toLeft = 960;
	flip_attack = 22;
	flip_attack_from_rightside = 439;
	flip_idle = 27;
	flip_idle_to_left = 557;
	when_to_fallX = 25;
	when_to_fallY = 41;
	flip_block = 30;
	flip_dead_from_rightside = 690;
	flip_dead_from_leftside = 16;
	window_height = 1080;
	window_width = 1920;
	movementSpeed = 600.f;
	combo_animation = 0;

	is_left = true;
	is_on_ground = false;
	is_blocking = false;
	already_picked = false;
	was_chosen = false;
	stationary = false;
	dead = false;
	is_attacking = false;
	Player_is_on_rightside = false;
	Player_is_attacking = false;
	Player_is_moving = false;
	Player_has_attacked = false;
	Player_is_blocking = false;
	is_hit = false;

	velocity;
	pos = idle_sprite.getPosition();
	Player_pos;

	my_event.type = Event::KeyReleased;
	my_event.key.code = Keyboard::LControl;

	dt_clock;
	dt = dt_clock.restart().asSeconds();

	frameTimer;

	const float frameDuration = 16.7;

	elapsedTime;

	interpolationFactor;

}


	void SamuraiCommander::setAI(std::shared_ptr<Enemy> AI) {

		this->AI = AI;

	}

	float SamuraiCommander::lerp(float start, float end, float t) {

		return start + (end - start) * t;
	}

void SamuraiCommander::idleState() {
	
	    elapsedTime = frameTimer.getElapsedTime().asMilliseconds();

	    interpolationFactor = elapsedTime / frameDuration;

	    frameTimer.restart();

		pos = idle_sprite.getPosition();

		Player_pos = pos;

		Player_is_on_rightside = false;

		Player_is_moving = false;

		velocity.x = 0.f;
		velocity.y = 0.f;
		//------------------------------SamuraiCommander Idle-----------------------------------------------------		
		if (!is_left) {//looking at rightside

			idle_sprite.setTexture(idle);//space between frames is 73

			if (flip_idle == 27) {

				idle_sprite.setTextureRect(IntRect(flip_idle, 36, 55, 92));

				flip_idle = flip_idle + 128;
				
			}

			else if (flip_idle == 155) {

				idle_sprite.setTextureRect(IntRect(flip_idle, 36, 55, 92));

				flip_idle = flip_idle + 128;

			}

			else if (flip_idle == 283) {

				idle_sprite.setTextureRect(IntRect(flip_idle, 36, 55, 92));

				flip_idle = flip_idle + 128;

			}

			else if (flip_idle == 411) {

				idle_sprite.setTextureRect(IntRect(flip_idle, 36, 55, 92));

				flip_idle = flip_idle + 128;

			}

			else if (flip_idle == 539) {

				idle_sprite.setTextureRect(IntRect(flip_idle, 36, 55, 92));

				flip_idle = 27;

			}

		}

		if (is_left) {//space between frames is 73

			idle_sprite.setTexture(idle_left);

			if (flip_idle_to_left == 557) {

				idle_sprite.setTextureRect(IntRect(flip_idle_to_left, 36, 55, 92));

				flip_idle_to_left = flip_idle_to_left - 128;

			}

			else if (flip_idle_to_left == 429) {

				idle_sprite.setTextureRect(IntRect(flip_idle_to_left, 36, 55, 92));

				flip_idle_to_left = flip_idle_to_left - 128;

			}

			else if (flip_idle_to_left == 301) {

				idle_sprite.setTextureRect(IntRect(flip_idle_to_left, 36, 55, 92));

				flip_idle_to_left = flip_idle_to_left - 128;

			}

			else if (flip_idle_to_left == 173) {

				idle_sprite.setTextureRect(IntRect(flip_idle_to_left, 36, 55, 92));

				flip_idle_to_left = flip_idle_to_left - 128;

			}

			else if (flip_idle_to_left == 45) {

				idle_sprite.setTextureRect(IntRect(flip_idle_to_left, 36, 55, 92));

				flip_idle_to_left = 557;

			}

		}
}

void SamuraiCommander::movement() {

	//-------------------------------SamuraiCommander movement-------------------------------------------------
	

		if (Keyboard::isKeyPressed(Keyboard::D) && !stationary) {//moving to rightside

			is_left = false;

			velocity.x += movementSpeed * dt;

			idle_sprite.setTexture(run_to_rightside);

			if (flip == 10) {

				idle_sprite.setTextureRect(IntRect(flip, 39, 53, 89));

				flip = flip + 53 + 76;

			}

			else if (flip == 139) {

				idle_sprite.setTextureRect(IntRect(flip, 40, 53, 88));

				flip = flip + 53 + 75;

			}

			else if (flip == 267) {

				idle_sprite.setTextureRect(IntRect(flip, 41, 53, 87));

				flip = flip + 53 + 76;

			}

			else if (flip == 396) {

				idle_sprite.setTextureRect(IntRect(flip, 41, 53, 87));

				flip = flip + 53 + 74;

			}

			else if (flip == 523) {

				idle_sprite.setTextureRect(IntRect(flip, 38, 53, 90));

				flip = flip + 53 + 75;

			}

			else if (flip == 651) {

				idle_sprite.setTextureRect(IntRect(flip, 40, 53, 88));

				flip = flip + 53 + 75;

			}

			else if (flip == 779) {

				idle_sprite.setTextureRect(IntRect(flip, 41, 53, 87));

				flip = flip + 53 + 75;

			}

			else if (flip == 907) {

				idle_sprite.setTextureRect(IntRect(flip, 40, 53, 88));

				flip = 10;				
			}

			Player_is_moving = true;

		}

		if (Keyboard::isKeyPressed(Keyboard::A) && !stationary) {//moving to leftside

			is_left = true;

			velocity.x += -movementSpeed * dt;

			idle_sprite.setTexture(run_to_leftside);

			if (flip_toLeft == 960) {

				idle_sprite.setTextureRect(IntRect(flip_toLeft, 39, 53, 89));

				flip_toLeft = flip_toLeft - (53 + 76);

			}

			else if (flip_toLeft == 831) {

				idle_sprite.setTextureRect(IntRect(flip_toLeft, 40, 53, 88));

				flip_toLeft = flip_toLeft - (53 + 75);
				
			}

			else if (flip_toLeft == 703) {

				idle_sprite.setTextureRect(IntRect(flip_toLeft, 41, 53, 87));

				flip_toLeft = flip_toLeft - (53 + 76);

			}

			else if (flip_toLeft == 574) {

				idle_sprite.setTextureRect(IntRect(flip_toLeft, 41, 53, 87));

				flip_toLeft = flip_toLeft - (53 + 74);

			}

			else if (flip_toLeft == 447) {

				idle_sprite.setTextureRect(IntRect(flip_toLeft, 38, 53, 90));

				flip_toLeft = flip_toLeft - (53 + 75);

			}

			else if (flip_toLeft == 319) {

				idle_sprite.setTextureRect(IntRect(flip_toLeft, 40, 53, 88));

				flip_toLeft = flip_toLeft - (53 + 75);

			}

			else if (flip_toLeft == 191) {

				idle_sprite.setTextureRect(IntRect(flip_toLeft, 41, 53, 87));

				flip_toLeft = flip_toLeft - (53 + 75);

			}

			else if (flip_toLeft == 63) {

				idle_sprite.setTextureRect(IntRect(flip_toLeft, 40, 53, 88));

				flip_toLeft = 960;
			}

			Player_is_moving = true;
		}

		idle_sprite.move(velocity);
	
}

void SamuraiCommander::collision() {

	//-----------------------------SamuraiCommander Collision------------------------------------------------------
		//collision right
		if (pos.x + idle_sprite.getGlobalBounds().width > window_width) {

			idle_sprite.setPosition(window_width - idle_sprite.getGlobalBounds().width, pos.y);

		}//collision left
		if (pos.x < 0) {

			idle_sprite.setPosition(0, pos.y);

		}//collision bottom
		if (pos.y + idle_sprite.getGlobalBounds().height > window_height) {

			idle_sprite.setPosition(pos.x, window_height - idle_sprite.getGlobalBounds().height);

		}//collision top
		if (pos.y < 0) {

			idle_sprite.setPosition(pos.x, 0);
		}
	
}

void SamuraiCommander::combat() {

	//-----------------------------SamuraiCommander Combat------------------------------------------------------
	if (Keyboard::isKeyPressed(Keyboard::LControl) && buttonTimer.getElapsedTime().asMilliseconds() >= 450) {

		is_attacking = false;

		Player_has_attacked = true;

		buttonTimer.restart();

		combo_animation = combo_animation + 1;

		if (combo_animation > 3) {

			combo_animation = 1;
		}

	}

	if (my_event.type && my_event.key.code && !is_attacking && is_left && combo_animation == 1) {//----attack1 from rightside

		Player_is_attacking = true;
	
		idle_sprite.setTexture(attack_from_rightside1);

	    if (flip_attack_from_rightside == 439) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 35, 51, 93));

				flip_attack_from_rightside = flip_attack_from_rightside - (78 + 50);

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack_from_rightside == 311) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 35, 50, 93));

				flip_attack_from_rightside = flip_attack_from_rightside - (73 + 99);

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack_from_rightside == 139) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 39, 99, 89));

				flip_attack_from_rightside = flip_attack_from_rightside - (30 + 94);

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack_from_rightside == 15) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 39, 94, 89));

				//flip_attack_from_rightside = 439;

				flip_attack_from_rightside = 558;

				is_attacking = true;

				Player_is_attacking = false;

				elapsedTime = 0.f;

			}

		}

	}


	if (my_event.type && my_event.key.code && !is_attacking && !is_left && combo_animation == 1) {//----attack1 from leftside

		Player_is_attacking = true;

		idle_sprite.setTexture(attack_from_leftside1);

		if (flip_attack == 22) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 35, 51, 93));

				flip_attack = flip_attack + (78 + 50);

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack == 150) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 35, 50, 93));

				flip_attack = flip_attack + 50 + 74;

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack == 274) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 39, 99, 89));

				flip_attack = flip_attack + 99 + 30;

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack == 403) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 39, 94, 89));

				//flip_attack = 22;

				flip_attack = 24;

				is_attacking = true;

				Player_is_attacking = false;

				elapsedTime = 0.f;

			}

		}

	}

	if (my_event.type && my_event.key.code && !is_attacking && is_left && combo_animation == 2) {//----attack2 from rightside

		Player_is_attacking = true;

		idle_sprite.setTexture(attack_from_rightside2);

		if (flip_attack_from_rightside == 558) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 38, 57, 90));

				flip_attack_from_rightside = flip_attack_from_rightside - (72 + 53);

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack_from_rightside == 433) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 30, 53, 98));

				flip_attack_from_rightside = flip_attack_from_rightside - (75 + 51);

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack_from_rightside == 307) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 24, 51, 104));

				flip_attack_from_rightside = flip_attack_from_rightside - (79 + 65);

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack_from_rightside == 163) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 31, 65, 97));

				flip_attack_from_rightside = flip_attack_from_rightside - (71 + 85);

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack_from_rightside == 7) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 44, 85, 84));

				//flip_attack_from_rightside = 558;

				flip_attack_from_rightside = 438;

				is_attacking = true;

				Player_is_attacking = false;

				elapsedTime = 0.f;

			}

		}

	}

	if (my_second_event.type && my_second_event.key.code && !is_attacking && !is_left && combo_animation == 2) {//----attack2 from leftside

		Player_is_attacking = true;

		idle_sprite.setTexture(attack_from_leftside2);

		if (flip_attack == 24) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 38, 57, 90));

				flip_attack = flip_attack + (72 + 57);

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack == 153) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 30, 53, 98));

				flip_attack = flip_attack + (75 + 53);

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack == 281) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 24, 51, 104));

				flip_attack = flip_attack + (79 + 51);

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack == 411) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 31, 65, 97));

				flip_attack = flip_attack + (72 + 65);

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack == 548) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 44, 85, 84));

				//flip_attack = 24;

				flip_attack = 30;

				is_attacking = true;

				Player_is_attacking = false;

				elapsedTime = 0.f;

			}

		}

	}

	if (my_event.type && my_event.key.code && !is_attacking && is_left && combo_animation == 3) {//----attack3 from rightside

		Player_is_attacking = true;

		idle_sprite.setTexture(attack_from_rightside3);

		if (flip_attack_from_rightside == 438) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 44, 43, 84));

				flip_attack_from_rightside = flip_attack_from_rightside - (61 + 67);

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack_from_rightside == 310) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 40, 67, 88));

				flip_attack_from_rightside = flip_attack_from_rightside - (72 + 95);

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack_from_rightside == 143) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 38, 95, 90));

				flip_attack_from_rightside = flip_attack_from_rightside - (35 + 84);

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack_from_rightside == 24) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack_from_rightside, 38, 84, 90));

				//flip_attack_from_rightside = 438;

				flip_attack_from_rightside = 439;

				is_attacking = true;

				Player_is_attacking = false;

				elapsedTime = 0.f;

			}

		}

	}


	if (my_event.type && my_event.key.code && !is_attacking && !is_left && combo_animation == 3) {//----attack3 from leftside

		Player_is_attacking = true;

		idle_sprite.setTexture(attack_from_leftside3);

		if (flip_attack == 30) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 44, 43, 84));

				flip_attack = flip_attack + (62 + 43);

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack == 135) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 40, 67, 88));

				flip_attack = flip_attack + (72 + 67);

				elapsedTime = 0.f;

			}
		}

		else if (flip_attack == 274) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 38, 95, 90));

				flip_attack = flip_attack + (34 + 95);

				elapsedTime = 0.f;

			}

		}

		else if (flip_attack == 403) {

			if (elapsedTime >= frameDuration) {

				idle_sprite.setTextureRect(IntRect(flip_attack, 38, 84, 90));

				//flip_attack = 30;

				flip_attack = 22;

				is_attacking = true;

				Player_is_attacking = false;

				elapsedTime = 0.f;

			}

		}

	}

}
		

void SamuraiCommander::hitReaction() {

	if (is_hit && !is_left) {

		idle_sprite.setScale(5, 5);

		idle_sprite.setTexture(dead_from_leftside);

		idle_sprite.setTextureRect(IntRect(flip_dead_from_leftside, 69, 83, 135));

		sleep(milliseconds(55));

		flip_dead_from_rightside = flip_dead_from_leftside + 125;
		
		idle_sprite.setPosition(pos.x, pos.y - 10);

		if (flip_dead_from_leftside >= 690) {

			idle_sprite.setTextureRect(IntRect(690, 69, 83, 135));

			dead = true;

		}

	}

	else if (is_hit && is_left) {

		idle_sprite.setScale(5, 5);

		idle_sprite.setTexture(dead_from_rightside);

		idle_sprite.setTextureRect(IntRect(flip_dead_from_rightside, 69, 83, 135));

		sleep(milliseconds(55));

		flip_dead_from_rightside = flip_dead_from_rightside - 125;

		idle_sprite.setPosition(pos.x, pos.y - 10);

		if (flip_dead_from_rightside <= 16) {

			idle_sprite.setTextureRect(IntRect(14, 69, 83, 135));
			
			dead = true;

		}
	}
	
}

void SamuraiCommander::hitReaction(String Enemy) {

	if (idle_sprite.getGlobalBounds().intersects(AI->idle_sprite.getGlobalBounds()) && (Keyboard::isKeyPressed(Keyboard::Right))) {

		idle_sprite.setPosition(pos);

		AI->idle_sprite.setPosition(AI->pos);

		idleState();

	}

	else if (idle_sprite.getGlobalBounds().intersects(AI->idle_sprite.getGlobalBounds()) && (Keyboard::isKeyPressed(Keyboard::Left))) {

		idle_sprite.setPosition(pos);

		AI->idle_sprite.setPosition(AI->pos);

		idleState();

	}
	else if (idle_sprite.getGlobalBounds().intersects(AI->idle_sprite.getGlobalBounds())) {

		idle_sprite.setPosition(pos);

		AI->idle_sprite.setPosition(AI->pos);

	}

}


