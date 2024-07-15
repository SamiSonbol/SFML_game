#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <chrono>
#include <thread>
#include <string.h>
#include<vector>
#include "Aramusha.h"
#include "Enemy.h"
#include "Fluids.h"
#include "Physics.h"
#include "Grid.h"
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

	std::shared_ptr<Physics> physics_controller = std::make_shared<Physics>();

	//std::shared_ptr<Physics::engine> engine = std::make_shared<Physics::engine>();

	physics_controller->setWindow(&window);

	physics_controller->create_planet(150, 700, 900);

	physics_controller->create_planet(40, 1060, 900);

	physics_controller->create_planet(80, 500, 1060);

	physics_controller->create_border(true, window.getSize().x, 5, 0, window.getSize().y - 45);//lower border

	physics_controller->create_border(true, window.getSize().x, 5, 0, 0);//upper border

	physics_controller->create_border(false, 5, window.getSize().y, 0, 0);//left border

	physics_controller->create_border(false, 5, window.getSize().y, 1880, 0);//right border

	bool start = true;


	std::vector<std::vector<int>> game_grid_2DVector(window_height, std::vector<int>(window_width, 0));

	std::shared_ptr<Grid> game_grid = std::make_shared<Grid>(game_grid_2DVector);

	Background background = Background(12);

	std::shared_ptr<Fluids> Blood = std::make_shared<Fluids>();

	std::shared_ptr<Aramusha> aramusha = std::make_shared<Aramusha>();

	std::shared_ptr<Enemy> AI = std::make_shared<Enemy>();


	aramusha->setEnemy(AI);

	AI->setAramusha(aramusha);

	Blood->setWindow(&window);

	Blood->setGrid(game_grid);

	Blood->setEnemy(AI);

	Blood->setAramusha(aramusha);

	Blood->setPhysics(physics_controller);

	physics_controller->setFluids(Blood);


	//Blood
	Blood->start_blood_cut_thread();
	
	//Physics
	physics_controller->start_collider_control_thread(physics_controller->points, physics_controller->colliders);

	//window loop
	while (window.isOpen()) {

		dt = dt_clock.restart().asSeconds();

		sf::Event my_event;

		while (window.pollEvent(my_event)) {

			if (my_event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

				window.close();

			}

		}

		//background
		background.load_background_frames(12);
		
		//aramusha
		aramusha->live();

		//AI
		AI->live(1);

		physics_controller->TEST(&window, *physics_controller, start);

		physics_controller->UPDATE(dt, physics_controller->planets, physics_controller->borders, physics_controller->points, physics_controller->colliders, physics_controller->constraints);

		//window drawing logic
		window.clear(Color::Black);

		window.draw(background.moon_bamboo_forest);

		window.draw(Blood->shoji);

		window.draw(aramusha->idle_sprite);

		window.draw(AI->idle_sprite);

		Blood->myPool.Thread_Lock.lock();

		for (const auto& cells : Blood->Blood_cells) {

			window.draw(cells);

		}
		
		physics_controller->render_colliders(physics_controller->blood_points, physics_controller->blood_colliders);

		Blood->myPool.Thread_Lock.unlock(); 
		
		physics_controller->RENDER(physics_controller->points, physics_controller->colliders, physics_controller->constraints, physics_controller->Lines);

		window.display();

	}//window closing bracket


};