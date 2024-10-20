#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Animation.h"

class Background
{
public:

	Animator animator;

	Spritesheet image;
	
	sf::Sprite rendered_sprite;

	int fps;
	
	Background(const std::string& image_path, const sf::Vector2u& spritesheet_size, const int fps = 60);

	void RENDER(sf::RenderWindow* window, const float& delta_time);

};

