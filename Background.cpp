#include "Background.h"

Background::Background(const std::string& image_path, const sf::Vector2u& spritesheet_size, const int fps) : image(image_path, spritesheet_size) {

	this->fps = fps;

};

void Background::RENDER(sf::RenderWindow* window, const float& delta_time) {

	this->animator.update(this->rendered_sprite, this->image.texture, this->image.size, this->image.frame_size, this->image.sprite_scale, this->image.current_frame, this->image.elapsed_time, delta_time, this->fps);
	window->draw(this->rendered_sprite);

};