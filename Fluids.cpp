#include "Fluids.h"
#include "Grid.h"
#include "Aramusha.h"
#include "Enemy.h"
#include "Physics.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <mutex>
#include <chrono>

Fluids::ThreadPool::ThreadPool() : nThreads(0) {}

Fluids::ThreadPool::ThreadPool(std::size_t nThreads) {

	Blood_Pool;

	Condition;

	Thread_Lock;

	destroy = false;

	Tasks;

	run(nThreads);

}

void Fluids::ThreadPool::run(std::size_t nThreads) {

	for (auto i = 0; i < nThreads; i++) {

		this->Blood_Pool.emplace_back([=] {

			while (true) {

				Task task;

				{

					std::unique_lock<std::mutex> lock(Thread_Lock);

					Condition.wait(lock, [=] {


						return destroy || !Tasks.empty();


						});

					if (destroy && Tasks.empty()) {

						break;

					}

					task = std::move(Tasks.front());

					Tasks.pop();

				}

				task();

			}

		});

	}

}

void Fluids::ThreadPool::stop() {

	{

		std::unique_lock<std::mutex> lock(Thread_Lock);

		destroy = true;

	}

	Condition.notify_all();

	for (auto& thread : Blood_Pool) {

		thread.join();

	}

}

void Fluids::ThreadPool::Enque(Task task) {

	{

		std::unique_lock<std::mutex> lock(Thread_Lock);

		Tasks.emplace(std::move(task));

	}

	Condition.notify_one();

}

Fluids::ThreadPool::~ThreadPool() {

	stop();

}
//----------------------------------------------
Fluids::Fluids() : myPool(1) {

	Event Blood_event;

	blood_colliders_clock;

	Bleeding_out;

	cycle_done = false;

	Thread_done = false;

	isHeld = false;

	wasHeld = false;

	starting_pointX = 0;

	ending_pointX = 0;

	starting_pointY = 0;

	ending_pointY = 0;

	Bleeding_out_range;

	Bleeding_Range;

	movement_clock;

	movement_dt = movement_clock.restart().asSeconds();

	movement_speed = 400;

	shoji_wall.loadFromFile("shoji.png");

	shoji.setTexture(shoji_wall);

	shoji.setTextureRect(IntRect(3, 4, 355, 235));

	shoji.setPosition(100, 280);

	shoji.setScale(4, 2.5);

};

void Fluids::setAramusha(std::shared_ptr<Aramusha> aramusha) {

	this->aramusha = aramusha;

}

void Fluids::setEnemy(std::shared_ptr<Enemy> AI) {

	this->AI = AI;

	image = AI->idle.copyToImage();

	shoji_image = shoji_wall.copyToImage();

}

void Fluids::setGrid(std::shared_ptr<Grid> myGrid) {

	this->myGrid = myGrid;

}

void Fluids::setPhysics(std::shared_ptr<Physics> physics) {

	this->physics = physics;

}

void Fluids::setWindow(sf::RenderWindow* window) {

	this->window = window;

}

bool Fluids::compare(int a, int b, int s) {

	if (s == -1) {

		return a > b;

	}

	else if (s == 1) {

		return a < b;

	}

}

void Fluids::blood_intersection(int y, int x, int limit, int symbolX, int symbolY) {

	sf::Vector2f pixel_point(x, y);

	sf::Vector2f blood_splash(x, y);

	if (AI->idle_sprite.getGlobalBounds().contains(pixel_point)) {

			blood_splash_vector.push_back(blood_splash);

			this->limit = limit + 1;	

			int rand = std::rand() % 101;

			if (rand >= 70) {
				
				int next_limit = (limit + 1) % blood_splash_vector.size();

				sf::Vector2f Target = { blood_splash_vector[next_limit].x - blood_splash_vector[limit].x , blood_splash_vector[next_limit].y - blood_splash_vector[limit].y };

				float distance = physics->length(Target);

				blood_colliders_dt += 0.7;

				sf::Vector2f blood_velocity = { (distance / blood_colliders_dt), (distance / blood_colliders_dt) };

				if (rand >= 80) {

					blood_velocity.y = std::rand() % 750;

				}
				else if (rand >= 90) {

					blood_velocity.x = std::rand() % 750;

				}

				blood_velocity.x *= symbolX;

				blood_velocity.y *= symbolY;

				myPool.Thread_Lock.lock();

				physics->create_collider(2.6, blood_splash_vector[limit].x, blood_splash_vector[limit].y, blood_velocity, sf::Color::Red, physics->blood_points, physics->blood_colliders);

				myPool.Thread_Lock.unlock();

			}

	}

			blood_colliders_dt = blood_colliders_clock.restart().asSeconds();

			if (!physics->blood_colliders.empty() && !physics->blood_points.empty()) {

					physics->update_gravity_and_positions(blood_colliders_dt, physics->blood_points);

					physics->update_colliders(blood_colliders_dt, physics->blood_points, physics->blood_colliders);

			}

			for (int i = 0; i < physics->blood_points.size(); i++) {

				int rand = std::rand() % 100;

				sf::Vector2f stainPos = physics->blood_points[i].position;

				if (AI->idle_sprite.getGlobalBounds().contains(stainPos) && rand >= 65) {

					sf::Vector2f spriteCoords = AI->idle_sprite.getInverseTransform().transformPoint(stainPos);

					int on_sprite_target_x = static_cast<int>(spriteCoords.x);

					int on_sprite_target_y = static_cast<int>(spriteCoords.y);

					if (AI->is_left) {

						on_sprite_target_x -= 24;

						on_sprite_target_y -= 28;

					}
					else {

						on_sprite_target_x += 24;

						on_sprite_target_y += 28;

					}

					bool valid = true;

					if (on_sprite_target_x >= 40 && on_sprite_target_x <= 49 && 79 <= on_sprite_target_y && on_sprite_target_x <= 96) {

						valid = false;

					}

					if (on_sprite_target_x > 32 && on_sprite_target_y > 31 && valid) {

						//std::cout << "\n target.X: " << on_sprite_target_x << " target.Y: " << on_sprite_target_y;

						sf::Color PixelColor = image.getPixel(on_sprite_target_x, on_sprite_target_y);

						PixelColor.r = 192;

						PixelColor.g = std::max(PixelColor.g - 9, 0);

						image.setPixel(on_sprite_target_x, on_sprite_target_y, PixelColor);

						///
						PixelColor = image.getPixel(on_sprite_target_x + 96, on_sprite_target_y);

						PixelColor.r = 192;

						PixelColor.g = std::max(PixelColor.g - 9, 0);

						image.setPixel(on_sprite_target_x + 96, on_sprite_target_y, PixelColor);

						///
						PixelColor = image.getPixel(on_sprite_target_x + 96 + 96, on_sprite_target_y);

						PixelColor.r = 192;

						PixelColor.g = std::max(PixelColor.g - 9, 0);

						image.setPixel(on_sprite_target_x + 96 + 96, on_sprite_target_y, PixelColor);

						///
						PixelColor = image.getPixel(on_sprite_target_x + 96 + 96 + 96, on_sprite_target_y);

						PixelColor.r = 192;

						PixelColor.g = std::max(PixelColor.g - 9, 0);

						image.setPixel(on_sprite_target_x + 96 + 96 + 96, on_sprite_target_y, PixelColor);

						///
						PixelColor = image.getPixel(on_sprite_target_x + 96 + 96 + 96 + 96, on_sprite_target_y);

						PixelColor.r = 192;

						PixelColor.g = std::max(PixelColor.g - 9, 0);

						image.setPixel(on_sprite_target_x + 96 + 96 + 96 + 96, on_sprite_target_y, PixelColor);

						///
						PixelColor = image.getPixel(on_sprite_target_x + 96 + 96 + 96 + 96 + 96, on_sprite_target_y);

						PixelColor.r = 192;

						PixelColor.g = std::max(PixelColor.g - 9, 0);

						image.setPixel(on_sprite_target_x + 96 + 96 + 96 + 96 + 96, on_sprite_target_y, PixelColor);

					}

					AI->idle.update(image);

				}

			}		

			mirror_blood(limit, symbolX, symbolY);

}


void Fluids::mirror_blood(int limit, int symbolX, int symbolY) {

	std::vector<int> to_delete_indexes;

	for (int i = 0; i < physics->blood_points.size(); i++) {

		int rand = std::rand() % 101;

		if (rand >= 60) {

			sf::Vector2f stainPos = physics->blood_points[i].position;

			if (shoji.getGlobalBounds().contains(stainPos)) {

				if (stainPos.x >= 0 && stainPos.x <= 1919 && stainPos.y <= 1079 && stainPos.y >= 0) {

					sf::Vector2f shojiCoords = shoji.getInverseTransform().transformPoint(stainPos);

					int on_shoji_target_x = static_cast<int>(shojiCoords.x);

					int on_shoji_target_y = static_cast<int>(shojiCoords.y);

					shoji_image.setPixel(on_shoji_target_x, on_shoji_target_y, sf::Color::Red);

				}

					sf::Vector2f new_target = { stainPos.x - blood_splash_vector[0].x, stainPos.y - blood_splash_vector[0].y};

					if (physics->length(new_target) >= 300) {

						int divider = std::rand() % 2;

						if (divider == 0) {

							physics->blood_points[i].velocity = { -19 , 200 };

						}
						else if(divider == 1){

							physics->blood_points[i].velocity = { 0 , 200 };

						}
					    else {

						physics->blood_points[i].velocity = { 19, 200 };

						}
						
					}
					else if (physics->length(new_target) >= 350) {
						
						to_delete_indexes.push_back(i);

					}

			}	

		}

	}


	shoji_wall.update(shoji_image);

	if (!to_delete_indexes.empty()) {

			myPool.Thread_Lock.lock();

			for (int i = to_delete_indexes.size() - 1; i > -1; i--) {

				std::vector<sf::CircleShape> ::iterator it = physics->blood_colliders.begin();

				std::vector<Physics::point_mass> ::iterator it2 = physics->blood_points.begin();

				it += to_delete_indexes[i];

				it2 += to_delete_indexes[i];

				physics->blood_colliders.erase(it);

				physics->blood_points.erase(it2);

			}

			myPool.Thread_Lock.unlock();

    }

}

void Fluids::start_blood_burst(int origin_pointX, int origin_pointY) {




}

	void Fluids::start_blood_cut() {


		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			if (!isHeld.load()) {

				isHeld = true;

			}

		}

		else {

			if (isHeld.load()) {

				isHeld.store(false);

			}

			Thread_done.store(false);

		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

		if (isHeld.load()) {

			wasHeld.store(true);

			if (mousePos.x >= 0 && mousePos.x < 1920 && mousePos.y >= 0 && mousePos.y < 1080) {

				if (mousePos.y < 1080 && mousePos.x < myGrid->getNumberOfColumns(mousePos.y)) {

					int random = rand() % 100;

					int rand_endX = rand() % 50;

					int rand_endy = rand() % 50;

					myGrid->getField(mousePos.y, mousePos.x).setValue(1);

					myGrid->setBloodLength(mousePos.y, mousePos.x, random);

					if (!Thread_done.load()) {

						starting_pointX = mousePos.x;

						starting_pointY = mousePos.y;

						Thread_done.store(true);

					}

				}

			}

		}

		if (!isHeld.load() && wasHeld.load()) {

			ending_pointX = mousePos.x;

			ending_pointY = mousePos.y;

			wasHeld.store(false);

			start_blood_dripping_thread();

		}

}

void Fluids::start_blood_dripping() {

	int startY = starting_pointY;

	int startX = starting_pointX;

	int symbolX = 1;

	int symbolY = 1;

	if (starting_pointX > ending_pointX) {

		symbolX = -1;

	}

	if (starting_pointY > ending_pointY) {

		symbolY = -1;

	}

	/////

		for (int y = startY; compare(y, ending_pointY, symbolY); y = y + symbolY) {

			for (int x = startX; compare(x, ending_pointX, symbolX); x = x + symbolX) {

				if (y != 0 && x != 0) {

					if (myGrid->getRed(y - 1, x) && !myGrid->getDrawn(y - 1, x)) {

						int r = rand() % 10;

						if (r < 4 && r > 0) {

							myGrid->getField(y, x).setValue(1);

							//myGrid->setBloodLength(y, x, myGrid->getBloodLength(y - 1, x) - 1);

							myGrid->setDrawn(y - 1, x);


						}

						else if (r < 8 && r > 3) {

							if (y + 1 <= 1079 && x - 1 >= 0) {

								myGrid->getField(y + 1, x - 1).setValue(1);

								//myGrid->setBloodLength(y + 1, x - 1, myGrid->getBloodLength(y - 1, x) - 1);

								myGrid->setDrawn(y - 1, x);

							}

						}

						else if (r < 11 && r > 7) {

							if (y + 1 <= 1079 && x + 1 <= 1919) {

								myGrid->getField(y + 1, x + 1).setValue(1);

								//myGrid->setBloodLength(y + 1, x + 1, myGrid->getBloodLength(y - 1, x) - 1);

								myGrid->setDrawn(y - 1, x);

							}

						}

					}

					if (!myGrid->getRed(y, x) && myGrid->getField(y, x).getValue() == 1 && !myGrid->getDrawn(y, x)) {

						//Bleeding.restart();

						//sf::RectangleShape pixel;

					   // pixel.setFillColor(Color::Red);

						//pixel.setSize(sf::Vector2f(5, 5));

						//pixel.setPosition(static_cast<float>(x), static_cast<float>(y));

						myGrid->setRed(y, x);

						//if (pixel.getPosition().y == 1079) {

							//bloodLength.push_back(1);

						//}

						//myPool.Thread_Lock.lock();

						//Blood_cells.push_back(pixel);

						//myPool.Thread_Lock.unlock();

						blood_intersection(y, x, limit, symbolX, symbolY);

						//sf::Vector2f blood_splash(x, y);

						//blood_splash_vector.push_back(blood_splash);

						//mirror_blood(limit, symbolX, symbolY);

					}

				}

				Bleeding_out_range = Bleeding_Range.getElapsedTime().asSeconds();

				if (Bleeding_out_range >= 5.f) {

					cycle_done.store(true);

					myGrid->setDrawn(y - 1, x);

					myGrid->setRed(y - 1, x);

					myGrid->setDrawn(y, x);

					myGrid->setRed(y, x);

					limit = 0;

					myPool.Thread_Lock.lock();

					Blood_cells.clear();

					bloodLength.clear();

					myPool.Thread_Lock.unlock();

					break;

				}

			}
		
		}

	}


void Fluids::blood_dripping_thread() {

	Thread_done.store(true);

	first_contact_handling.store(false);

	std::cout << "\nthreadCreated   ";

	Bleeding_Range.restart();

	limit = 0;

	blood_splash_i = 0;

	n_of_bursts_i = 0;

	while (!cycle_done) {

		start_blood_dripping();

	}

	myPool.Thread_Lock.lock();

	physics->blood_colliders.clear();

	physics->blood_points.clear();

	myPool.Thread_Lock.unlock();

	blood_splash_vector.clear();

	n_of_bursts_vector.clear();

	int startY = starting_pointY;

	int startX = starting_pointX;

	int symbolX = 1;

	int symbolY = 1;



	if (starting_pointX > ending_pointX) {

		symbolX = -1;

	}

	if (starting_pointY > ending_pointY) {

		symbolY = -1;

	}

	for (int y = startY; compare(y, ending_pointY, symbolY); y = y + symbolY) {

		for (int x = startX; compare(x, ending_pointX, symbolX); x = x + symbolX) {

			if (y != 0 && x != 0) {

					if (myGrid->getField(y, x).getValue() == 1 && (!myGrid->getDrawn(y, x) || myGrid->getRed(y, x))) {

						myGrid->setDrawn(y, x);

						myGrid->setRed(y, x);

					}

			}

		}

	}	

	cycle_done.store(false);

	std::cout << "\nthread broke";

}

void Fluids::start_blood_dripping_thread() {

	if (!Thread_done) {

		myPool.Enque([this] {

			blood_dripping_thread();

			});

	}

}

void Fluids::blood_cut_thread()
{

	while (true) {

		start_blood_cut();

	}

}

void Fluids::start_blood_cut_thread()
{

	std::thread Blood_drop([this]() {

		this->blood_cut_thread();

		});

	Blood_drop.detach();

}



