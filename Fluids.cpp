//#include "Fluids.h"
//
//Fluids::ThreadPool::ThreadPool() : nThreads(0) {}
//
//Fluids::ThreadPool::ThreadPool(std::size_t nThreads) {
//
//	Blood_Pool;
//
//	Condition;
//
//	Thread_Lock;
//
//	destroy = false;
//
//	Tasks;
//
//	run(nThreads);
//
//}
//
//void Fluids::ThreadPool::run(std::size_t nThreads) {
//
//	for (auto i = 0; i < nThreads; i++) {
//
//		this->Blood_Pool.emplace_back([=] {
//
//			while (true) {
//
//				Task task;
//
//				{
//
//					std::unique_lock<std::mutex> lock(Thread_Lock);
//
//					Condition.wait(lock, [=] {
//
//
//						return destroy || !Tasks.empty();
//
//
//						});
//
//					if (destroy && Tasks.empty()) {
//
//						break;
//
//					}
//
//					task = std::move(Tasks.front());
//
//					Tasks.pop();
//
//				}
//
//				task();
//
//			}
//
//		});
//
//	}
//
//}
//
//void Fluids::ThreadPool::stop() {
//
//	{
//
//		std::unique_lock<std::mutex> lock(Thread_Lock);
//
//		destroy = true;
//
//	}
//
//	Condition.notify_all();
//
//	for (auto& thread : Blood_Pool) {
//
//		thread.join();
//
//	}
//
//}
//
//void Fluids::ThreadPool::Enque(Task task) {
//
//	{
//
//		std::unique_lock<std::mutex> lock(Thread_Lock);
//
//		Tasks.emplace(std::move(task));
//
//	}
//
//	Condition.notify_one();
//
//}
//
//Fluids::ThreadPool::~ThreadPool() {
//
//	stop();
//
//}
////----------------------------------------------
//Fluids::Fluids() : myPool(1) {
//
//	Event Blood_event;
//
//	blood_colliders_clock;
//
//	Bleeding_out;
//
//	cycle_done = false;
//
//	Thread_done = false;
//
//	isHeld = false;
//
//	wasHeld = false;
//
//	starting_pointX = 0;
//
//	ending_pointX = 0;
//
//	starting_pointY = 0;
//
//	ending_pointY = 0;
//
//	Bleeding_out_range;
//
//	Bleeding_Range;
//
//	shoji_wall.loadFromFile("resources\\Backgrounds\\shoji.png");
//
//	shoji_image.loadFromFile("resources\\Backgrounds\\shoji.png");
//
//	shoji.setTexture(shoji_wall);
//
//	shoji.setTextureRect(IntRect(3, 4, 355, 235));
//
//	shoji.setPosition(100, 280);
//
//	shoji.setScale(4, 2.5);
//
//};
//
//bool Fluids::compare(int a, int b, int s) {
//
//	if (s == -1) {
//
//		return a > b;
//
//	}
//
//	else if (s == 1) {
//
//		return a < b;
//
//	}
//
//};
//
//void Fluids::blood_intersection(Physics& physics, sf::Sprite& sprite, int y, int x, const float delta_time, int symbolX, int symbolY) {
//
//	if (sprite.getGlobalBounds().contains(sf::Vector2f(x, y))) {
//
//		this->image = sprite.getTexture()->copyToImage();
//
//		blood_splash_vector.push_back(sf::Vector2f(x, y));
//
//		this->limit += 1;
//
//		int rand = std::rand() % 101;
//
//		if (rand >= 70) {
//
//			int index = (this->limit + 1) % blood_splash_vector.size();
//
//			vec2 Target(blood_splash_vector[index].x - blood_splash_vector[limit].x , blood_splash_vector[index].y - blood_splash_vector[limit].y);
//
//			float distance = Target.magnitude();
//
//			sf::Vector2f blood_velocity((distance / delta_time), (distance / delta_time));
//
//			if (rand >= 80) {
//
//				blood_velocity.y = std::rand() % 750;
//
//			}
//			else if (rand >= 90) {
//
//				blood_velocity.x = std::rand() % 750;
//
//			}
//
//			blood_velocity.x *= symbolX;
//
//			blood_velocity.y *= symbolY;
//
//			myPool.Thread_Lock.lock();
//
//			//physics.create_collider(2.6, blood_splash_vector[limit].x, blood_splash_vector[limit].y, blood_velocity, sf::Color::Red, physics.blood_points, physics.blood_colliders);
//
//			myPool.Thread_Lock.unlock();
//
//		};
//
//	};
//
//	blood_colliders_dt = blood_colliders_clock.restart().asSeconds();
//
//	if (!physics.blood_colliders.empty() && !physics.blood_points.empty()) {
//
//		//physics.update_gravity_and_positions(blood_colliders_dt, physics.blood_points);
//
//		//physics.update_colliders(blood_colliders_dt, physics.blood_points, physics.blood_colliders);
//
//	};
//
//	for (int i = 0; i < physics.blood_points.size(); i++) {
//
//		int rand = std::rand() % 101;
//
//		sf::Vector2f stainPos = { physics.blood_points[i].position.x, physics.blood_points[i].position.y };
//
//		if (sprite.getGlobalBounds().contains(stainPos) && rand >= 65) {
//
//			sf::Vector2f spriteCoords = sprite.getInverseTransform().transformPoint(stainPos);
//
//			int on_sprite_target_x = static_cast<int>(spriteCoords.x);
//
//			int on_sprite_target_y = static_cast<int>(spriteCoords.y);
//
//			//if (AI->is_left) {
//
//				on_sprite_target_x -= 24;
//
//				on_sprite_target_y -= 28;
//
//			//}
//			//else {
//
//				/*on_sprite_target_x += 24;
//
//				on_sprite_target_y += 28;*/
//
//			//}
//
//			bool valid = true;
//
//			if (on_sprite_target_x >= 40 && on_sprite_target_x <= 49 && 79 <= on_sprite_target_y && on_sprite_target_x <= 96) {
//
//				valid = false;
//
//			}
//
//			if (on_sprite_target_x > 32 && on_sprite_target_y > 31 && valid) {
//
//				//std::cout << "\n target.X: " << on_sprite_target_x << " target.Y: " << on_sprite_target_y;
//
//				sf::Color PixelColor = image.getPixel(on_sprite_target_x, on_sprite_target_y);
//
//				PixelColor.r = 192;
//
//				PixelColor.g = std::max(PixelColor.g - 9, 0);
//
//				image.setPixel(on_sprite_target_x, on_sprite_target_y, PixelColor);
//
//				///
//				PixelColor = image.getPixel(on_sprite_target_x + 96, on_sprite_target_y);
//
//				PixelColor.r = 192;
//
//				PixelColor.g = std::max(PixelColor.g - 9, 0);
//
//				image.setPixel(on_sprite_target_x + 96, on_sprite_target_y, PixelColor);
//
//				///
//				PixelColor = image.getPixel(on_sprite_target_x + 96 + 96, on_sprite_target_y);
//
//				PixelColor.r = 192;
//
//				PixelColor.g = std::max(PixelColor.g - 9, 0);
//
//				image.setPixel(on_sprite_target_x + 96 + 96, on_sprite_target_y, PixelColor);
//
//				///
//				PixelColor = image.getPixel(on_sprite_target_x + 96 + 96 + 96, on_sprite_target_y);
//
//				PixelColor.r = 192;
//
//				PixelColor.g = std::max(PixelColor.g - 9, 0);
//
//				image.setPixel(on_sprite_target_x + 96 + 96 + 96, on_sprite_target_y, PixelColor);
//
//				///
//				PixelColor = image.getPixel(on_sprite_target_x + 96 + 96 + 96 + 96, on_sprite_target_y);
//
//				PixelColor.r = 192;
//
//				PixelColor.g = std::max(PixelColor.g - 9, 0);
//
//				image.setPixel(on_sprite_target_x + 96 + 96 + 96 + 96, on_sprite_target_y, PixelColor);
//
//				///
//				PixelColor = image.getPixel(on_sprite_target_x + 96 + 96 + 96 + 96 + 96, on_sprite_target_y);
//
//				PixelColor.r = 192;
//
//				PixelColor.g = std::max(PixelColor.g - 9, 0);
//
//				image.setPixel(on_sprite_target_x + 96 + 96 + 96 + 96 + 96, on_sprite_target_y, PixelColor);
//
//			};
//
//			sf::Texture txt;
//			txt.loadFromImage(this->image);
//
//			//txt.update(this->image);
//
//			//sprite.setTexture(txt);
//
//		};
//
//	};
//
//	mirror_blood(physics, limit, symbolX, symbolY);
//
//};
//
//
//void Fluids::mirror_blood(Physics& physics, int limit, int symbolX, int symbolY) {
//
//	std::vector<int> to_delete_indexes;
//
//	for (int i = 0; i < physics.blood_points.size(); i++) {
//
//		int rand = std::rand() % 101;
//
//		if (rand >= 60) {
//
//			sf::Vector2f stainPos = { physics.blood_points[i].position.x, physics.blood_points[i].position.y };
//
//			if (shoji.getGlobalBounds().contains(stainPos)) {
//
//				if (stainPos.x >= 0 && stainPos.x <= 1919 && stainPos.y <= 1079 && stainPos.y >= 0) {
//
//					sf::Vector2f shojiCoords = shoji.getInverseTransform().transformPoint(stainPos);
//
//					int on_shoji_target_x = static_cast<int>(shojiCoords.x);
//
//					int on_shoji_target_y = static_cast<int>(shojiCoords.y);
//
//					shoji_image.setPixel(on_shoji_target_x, on_shoji_target_y, sf::Color::Red);
//
//				}
//
//					vec3 new_target(stainPos.x - blood_splash_vector[0].x, stainPos.y - blood_splash_vector[0].y);
//
//					if (new_target.magnitude() >= 300) {
//
//						int divider = std::rand() % 2;
//
//						if (divider == 0) {
//
//							physics.blood_points[i].velocity = { -19 , 200 };
//
//						}
//						else if(divider == 1){
//
//							physics.blood_points[i].velocity = { 0 , 200 };
//
//						}
//					    else {
//
//						physics.blood_points[i].velocity = { 19, 200 };
//
//						}
//						
//					}
//					else if (new_target.magnitude() >= 350) {
//						
//						to_delete_indexes.push_back(i);
//
//					}
//
//			}	
//
//		}
//
//	}
//
//
//	shoji_wall.update(shoji_image);
//
//	if (!to_delete_indexes.empty()) {
//
//			myPool.Thread_Lock.lock();
//
//			for (int i = to_delete_indexes.size() - 1; i > -1; i--) {
//
//				std::vector<sf::CircleShape> ::iterator it = physics.blood_colliders.begin();
//
//				std::vector<Physics::point_mass> ::iterator it2 = physics.blood_points.begin();
//
//				it += to_delete_indexes[i];
//
//				it2 += to_delete_indexes[i];
//
//				physics.blood_colliders.erase(it);
//
//				physics.blood_points.erase(it2);
//
//			}
//
//			myPool.Thread_Lock.unlock();
//
//    }
//
//}
//
//void Fluids::start_blood_burst(int origin_pointX, int origin_pointY) {
//
//
//
//
//}
//
//void Fluids::start_blood_cut(Physics& physics, Grid& grid, sf::RenderWindow* window, sf::Sprite& sprite, const float delta_time) {
//
//
//	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
//
//		if (!isHeld.load()) {
//
//			isHeld = true;
//
//		}
//
//	}
//
//	else {
//
//		if (isHeld.load()) {
//
//			isHeld.store(false);
//
//		}
//
//		Thread_done.store(false);
//
//	}
//
//	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
//
//	if (isHeld.load()) {
//
//		wasHeld.store(true);
//
//		if (mousePos.x >= 0 && mousePos.x < 1920 && mousePos.y >= 0 && mousePos.y < 1080) {
//
//			if (mousePos.y < 1080 && mousePos.x < grid.get_size().x) {//this->grid.get_size(mousePos.y)
//
//				int random = rand() % 101;
//
//				int rand_endX = rand() % 51;
//
//				int rand_endy = rand() % 51;
//
//				grid.setValue(mousePos.y, mousePos.x, 1);
//
//				//grid.setBloodLength(mousePos.y, mousePos.x, random);
//
//				if (!Thread_done.load()) {
//
//					starting_pointX = mousePos.x;
//
//					starting_pointY = mousePos.y;
//
//					Thread_done.store(true);
//
//				}
//
//			}
//
//		}
//
//	}
//
//	if (!isHeld.load() && wasHeld.load()) {
//
//		ending_pointX = mousePos.x;
//
//		ending_pointY = mousePos.y;
//
//		wasHeld.store(false);
//
//		start_blood_dripping_thread(physics, grid, sprite, delta_time);
//
//	};
//
//};
//
//void Fluids::start_blood_dripping(Physics& physics, Grid& grid, sf::Sprite& sprite, const float delta_time) {
//
//	int startY = starting_pointY;
//
//	int startX = starting_pointX;
//
//	int symbolX = 1;
//
//	int symbolY = 1;
//
//	if (starting_pointX > ending_pointX) {
//
//		symbolX = -1;
//
//	}
//
//	if (starting_pointY > ending_pointY) {
//
//		symbolY = -1;
//
//	}
//
//	/////
//
//		for (int y = startY; compare(y, ending_pointY, symbolY); y = y + symbolY) {
//
//			for (int x = startX; compare(x, ending_pointX, symbolX); x = x + symbolX) {
//
//				if (y != 0 && x != 0) {
//
//					if (grid.getRed(y - 1, x) && !grid.getDrawn(y - 1, x)) {
//
//						int r = rand() % 11;
//
//						if (r < 4 && r > 0) {
//
//							grid.setValue(y, x, 1);
//
//							//this->grid.setBloodLength(y, x, this->grid.getBloodLength(y - 1, x) - 1);
//
//							grid.setDrawn(y - 1, x);
//
//
//						}
//
//						else if (r < 8 && r > 3) {
//
//							if (y + 1 <= 1079 && x - 1 >= 0) {
//
//								grid.setValue(y + 1, x - 1, 1);
//
//								//this->grid.setBloodLength(y + 1, x - 1, this->grid.getBloodLength(y - 1, x) - 1);
//
//								grid.setDrawn(y - 1, x);
//
//							}
//
//						}
//
//						else if (r < 11 && r > 7) {
//
//							if (y + 1 <= 1079 && x + 1 <= 1919) {
//
//								grid.setValue(y + 1, x + 1, 1);
//
//								//this->grid.setBloodLength(y + 1, x + 1, this->grid.getBloodLength(y - 1, x) - 1);
//
//								grid.setDrawn(y - 1, x);
//
//							}
//
//						}
//
//					}
//
//					if (!grid.getRed(y, x) && grid.getValue(y, x) == 1 && !grid.getDrawn(y, x)) {
//
//						//Bleeding.restart();
//
//						//sf::RectangleShape pixel;
//
//					   // pixel.setFillColor(Color::Red);
//
//						//pixel.setSize(sf::Vector2f(5, 5));
//
//						//pixel.setPosition(static_cast<float>(x), static_cast<float>(y));
//
//					    grid.setRed(y, x);
//					    
//						//if (pixel.getPosition().y == 1079) {
//
//							//bloodLength.push_back(1);
//
//						//}
//
//						//myPool.Thread_Lock.lock();
//
//						//Blood_cells.push_back(pixel);
//
//						//myPool.Thread_Lock.unlock();
//
//						blood_intersection(physics, sprite, y, x, delta_time, symbolX, symbolY);
//
//						//sf::Vector2f blood_splash(x, y);
//
//						//blood_splash_vector.push_back(blood_splash);
//
//						//mirror_blood(limit, symbolX, symbolY);
//
//					}
//
//				}
//
//				Bleeding_out_range = Bleeding_Range.getElapsedTime().asSeconds();
//
//				if (Bleeding_out_range >= 5.f) {
//
//					cycle_done.store(true);
//
//					grid.setDrawn(y - 1, x);
//
//					grid.setRed(y - 1, x);
//
//					grid.setDrawn(y, x);
//
//					grid.setRed(y, x);
//
//					limit = 0;
//
//					myPool.Thread_Lock.lock();
//
//					Blood_cells.clear();
//
//					bloodLength.clear();
//
//					myPool.Thread_Lock.unlock();
//
//					break;
//
//				};
//
//			};
//		
//		};
//
//};
//
//
//void Fluids::blood_dripping_thread(Physics& physics, Grid& grid, sf::Sprite& sprite, const float delta_time) {
//
//	Thread_done.store(true);
//
//	first_contact_handling.store(false);
//
//	std::cout << "\nthreadCreated   ";
//
//	Bleeding_Range.restart();
//
//	limit = 0;
//
//	blood_splash_i = 0;
//
//	n_of_bursts_i = 0;
//
//	while (!cycle_done) {
//
//		start_blood_dripping(physics, grid, sprite, delta_time);
//
//	};
//
//	myPool.Thread_Lock.lock();
//
//	physics.blood_colliders.clear();
//
//	physics.blood_points.clear();
//
//	myPool.Thread_Lock.unlock();
//
//	blood_splash_vector.clear();
//
//	n_of_bursts_vector.clear();
//
//	int startY = starting_pointY;
//
//	int startX = starting_pointX;
//
//	int symbolX = 1;
//
//	int symbolY = 1;
//
//
//
//	if (starting_pointX > ending_pointX) {
//
//		symbolX = -1;
//
//	}
//
//	if (starting_pointY > ending_pointY) {
//
//		symbolY = -1;
//
//	}
//
//	for (int y = startY; compare(y, ending_pointY, symbolY); y = y + symbolY) {
//
//		for (int x = startX; compare(x, ending_pointX, symbolX); x = x + symbolX) {
//
//			if (y != 0 && x != 0) {
//
//					if (grid.getValue(x, y) == 1 && (!grid.getDrawn(x, y) || grid.getRed(x, y))) {
//
//						grid.setDrawn(x, y);
//
//						grid.setRed(x, y);
//
//					}
//
//			}
//
//		}
//
//	}	
//
//	cycle_done.store(false);
//
//	std::cout << "\nthread broke";
//
//}
//
//void Fluids::start_blood_dripping_thread(Physics& physics, Grid& grid, sf::Sprite& sprite, const float delta_time) {
//
//	if (!Thread_done) {
//
//		myPool.Enque([this, &physics, &grid, &sprite, &delta_time] {
//
//			blood_dripping_thread(physics, grid, sprite, delta_time);
//
//			});
//
//	}
//
//}
//
//void Fluids::blood_cut_thread(Physics& physics, Grid& grid, sf::RenderWindow* window, sf::Sprite& sprite, const float delta_time)
//{
//
//	while (true) {
//
//		start_blood_cut(physics, grid, window, sprite, delta_time);
//
//	}
//
//}
//
//void Fluids::start_blood_cut_thread(Physics& physics, Grid& grid, sf::RenderWindow* window, sf::Sprite& sprite, const float delta_time)
//{
//
//	std::thread Blood_drop([this, &physics, &grid, window, &sprite, &delta_time]() {
//
//		this->blood_cut_thread(physics, grid, window, sprite, delta_time);
//
//		});
//
//	Blood_drop.detach();
//
//}
//
//
//
