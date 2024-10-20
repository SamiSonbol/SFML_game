#include "Aramusha.h"

Aramusha::Aramusha() {

	this->sprite_scale = { 4, 4 };
	update_sprite(this->idle, this->main_sprite, this->sprite_scale, 1);
	main_sprite.setPosition(1920, 575);

	window_height = 1080;
	window_width = 1920;
	movementSpeed = 600.f;

	is_on_ground = false;
	already_picked = false;
	was_chosen = false;
	stationary = false;

	Player_is_attacking = false;
	Player_is_moving = false;
	Player_has_attacked = false;

	my_event.type = Event::KeyReleased;
	my_event.key.code = Keyboard::Space;

	bloodyAramusha = idle_left.texture.copyToImage();

};

//void Aramusha::setSamuraiCommander(std::shared_ptr<SamuraiCommander> commander) {
//
//	this->commander = commander;
//}
//
//void Aramusha::setEnemy(std::shared_ptr<Enemy> AI) {
//
//	this->AI = AI;
//}


void Aramusha::idleState(const float& delta_time) {

	Player_is_moving = false;

	velocity.x = 0.f;
	velocity.y = 0.f;

	if (!is_left) {

		update_sprite(this->idle, this->main_sprite, this->sprite_scale, delta_time);

	};

	if (is_left) {

		update_sprite(this->idle_looking_at_left_side, this->main_sprite, this->sprite_scale, delta_time);

	};

};

void Aramusha::movement(const float& delta_time) {

	//-------------------------------Aramusha movement-------------------------------------------------
	if (Keyboard::isKeyPressed(Keyboard::Right) && !stationary) {//moving to rightside

		is_left = false;

		velocity.x += movementSpeed * delta_time;

		Player_is_moving = true;

		update_sprite(this->run_to_rightside, this->main_sprite, this->sprite_scale, delta_time);
	};

	if (Keyboard::isKeyPressed(Keyboard::Left) && !stationary) {//moving to leftside

		is_left = true;

		velocity.x += -movementSpeed * delta_time;

		Player_is_moving = true;

		update_sprite(this->run_to_leftside, this->main_sprite, this->sprite_scale, delta_time);
	};

	main_sprite.move(velocity);

};

void Aramusha::combat(const float& delta_time) {

	//--------------------------Aramusha Combat---------------------------------------
	if (Keyboard::isKeyPressed(Keyboard::Space)) {

		is_attacking = false;

		Player_has_attacked = true;

	};

	if (my_event.type && my_event.key.code && !is_attacking && is_left) {//----attack1 from rightside

		Player_is_attacking = true;

		is_attacking = true;

		Player_is_attacking = false;

		update_sprite(this->attack_from_rightside1, this->main_sprite, this->sprite_scale, delta_time);

	};

	if (my_event.type && my_event.key.code && !is_attacking && !is_left) {//----attack1 from leftside

		Player_is_attacking = true;

		is_attacking = true;

		update_sprite(this->attack_from_leftside1, this->main_sprite, this->sprite_scale, delta_time);

	};

};

void Aramusha::live(Fighter& enemy_fighter, const float& delta_time) {

	idleState(delta_time);

	movement(delta_time);

	//screen_collision();

	//fighter_collision(enemy_fighter);

	combat(delta_time);

};
