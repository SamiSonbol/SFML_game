#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <Math.h>
#include <algorithm>
#include <chrono>
#include <thread>
#include <string.h>
#include <vector>
#include "Fighter.h"
#include "Grid.h"
#include "Aramusha.h"
#include "Enemy.h"
#include "Fluids.h"
#include "Physics.h"
#include "Background.h"

int main() {

	int window_height = 1080;
	int window_width = 1920;
	float dt;
	sf::Clock dt_clock;
	float movementSpeed = 600.f;

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "game", sf::Style::Fullscreen);//RENDERER

	window.setFramerateLimit(60);
	//////------------------------------------------------------------------------------------------------------------------------------------------
	/////-------------------------------------------------------------------------------------------------------------------------------------------
	//////------------------------------------------------------------------------------------------------------------------------------------------
	/////-------------------------------------------------------------------------------------------------------------------------------------------
	vec3 gravity(0, 9.81, 0);
	std::vector<vec3> positions = { vec3(500, 500), vec3(700, 500), vec3(700, 700), vec3(500, 700) };
	Physics physics_controller;
	physics_controller.Bodies.emplace_back(positions, Physics::Material::wood(), 500.0f, 2.0f);
	physics_controller.Bodies[0].frame.setFillColor(sf::Color(250, 0, 0, 250));
	physics_controller.Bodies[0].index = physics_controller.Bodies.size() - 1;

	bool start = true;


	std::vector<std::vector<int>> game_grid_2DVector(window_height, std::vector<int>(window_width, 0));
	Grid game_grid = Grid(game_grid_2DVector);

	//Fluids Blood;

	Background background = Background("resources\\Backgrounds\\moon_bamboo_forest.png", {5, 8});

	Aramusha aramusha = Aramusha();

	Enemy AI = Enemy();

	//Blood->setWindow(&window);

	//Blood->setGrid(game_grid);

	//Blood->setEnemy(AI);

	//Blood->setAramusha(aramusha);

	//Blood->setPhysics(physics_controller);

	//physics_controller->setFluids(Blood);


	//Blood
	//Blood.start_blood_cut_thread(physics_controller, game_grid, &window, AI.main_sprite);
	
	//Physics
	//physics_controller.start_collider_control_thread(&window, physics_controller.points, physics_controller.colliders);
	//window loop
	while (window.isOpen()) {

		dt = dt_clock.restart().asSeconds();

		sf::Event my_event;

		while (window.pollEvent(my_event)) {

			if (my_event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

				window.close();

			};

		};
		
		//aramusha
		aramusha.live(AI, dt);

		//AI
		AI.live(aramusha, dt);
		
		physics_controller.collider_control(&window);
		physics_controller.TEST(&window, gravity, start);
		physics_controller.UPDATE(dt, gravity);

		//window drawing logic
		window.clear(Color::Black);

		background.RENDER(&window, dt);

		//window.draw(Blood.shoji);

		aramusha.RENDER(&window, dt);

		AI.RENDER(&window, dt);

		physics_controller.RENDER(&window);

		//Blood.myPool.Thread_Lock.lock();

		//for (const auto& cells : Blood.Blood_cells) {

			//window.draw(cells);

		//}

		//Blood.myPool.Thread_Lock.unlock();

		window.display();

	}//window closing bracket


};