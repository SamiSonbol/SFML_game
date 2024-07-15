#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>

class Grid;

class Aramusha;

class Enemy;

class Physics;

class Fluids : public std::enable_shared_from_this<Fluids>
{

public:

	class ThreadPool {

	public:

		bool destroy;

		std::size_t nThreads;

		std::vector<std::thread> Blood_Pool;

		std::condition_variable Condition;

		std::mutex Thread_Lock;

		using Task = std::function<void()>;

		std::queue<Task> Tasks;

		ThreadPool();

		ThreadPool(std::size_t nThreads);

		~ThreadPool();

		void Enque(Task task);

		void run(std::size_t nThreads);

		void stop();

		ThreadPool(const ThreadPool&) = delete;

		ThreadPool& operator=(const ThreadPool&) = delete;

	}; // ThreadPool brackets

	Fluids();

	void setWindow(sf::RenderWindow* window);

	void setGrid(std::shared_ptr<Grid> myGrid);

	void setAramusha(std::shared_ptr<Aramusha> aramusha);

	void setEnemy(std::shared_ptr<Enemy> AI);

	void setPhysics(std::shared_ptr<Physics> physics);

	bool compare(int a, int b, int s);

	void mirror_blood(int limit, int symbolX, int symbolY);

	void start_blood_burst(int origin_pointX, int origin_pointY);

	void blood_intersection(int y, int x, int limit, int symbolX, int symbolY);

	std::atomic<bool> first_contact;

	std::atomic<bool> first_contact_handling;

	sf::Vector2f first_contact_coordinates;

	int limit;

	int blood_splash_i;

	int n_of_bursts_i;

	sf::Vector2f left_point;

	sf::Vector2f right_point;

	sf::Vector2f left_connector_point_buffer;

	sf::Vector2f right_connector_point_buffer;

	sf::Vector2f buffer_burst_connector_point;

	std::vector<sf::Vector2f> blood_splash_vector;//stores the points responsible for mirroring blood

	std::vector<sf::Vector2f> n_of_bursts_vector;//stores the points in the midle of a burst

	std::vector<sf::Vector2f> buffer_n_of_bursts_vector;


	sf::Sprite shoji;

	sf::Texture shoji_wall;

	sf::Image shoji_image;


	void start_blood_cut();

	void blood_cut_thread();

	void start_blood_cut_thread();

	void start_blood_dripping();

	void blood_dripping_thread();

	void start_blood_dripping_thread();

	ThreadPool myPool;

	std::mutex BloodMutex;

	sf::Event Blood_event;

	sf::Vector2i mousePos;

	sf::Clock blood_colliders_clock;

	sf::Clock Bleeding_Range;

	sf::Clock movement_clock;

	sf::Vector2f blood_velocity;

	float movement_speed;

	float movement_dt;

	float Bleeding_out_range;

	float Bleeding_out;

	float blood_colliders_dt;

	std::atomic<int> starting_pointX;

    std::atomic<int> ending_pointX;

	std::atomic<int> starting_pointY;

	std::atomic<int> ending_pointY;

	std::atomic<int> cellCounter;

	std::atomic<bool>  Thread_done;

	std::atomic<bool> cycle_done;

	std::atomic<bool> isHeld;

	std::atomic<bool> wasHeld;

	std::atomic<bool> mouseReleased;

	std::atomic<bool> blood_started;

	std::atomic<bool> mouse_clicked;

	sf::Image image;

	std::vector<sf::RectangleShape> Blood_cells;

	std::vector<int> bloodLength;

private:

	std::shared_ptr<Grid> myGrid;

	std::shared_ptr<Aramusha> aramusha;

	std::shared_ptr<Enemy> AI;

	std::shared_ptr<Physics> physics;

	sf::RenderWindow* window;

};

