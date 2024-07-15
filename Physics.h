#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Fluids;

class Physics : public std::enable_shared_from_this<Physics>
{

public:

    struct point_mass {
    
        sf::Vector2f position;

        sf::Vector2f velocity;

        float mass = 5;

    };

    struct collision {

        sf::Vector2f normal{ 0.f, 0.f };

        float depth = -std::numeric_limits<float>::infinity();

    };

    struct distance_constraint {

        uint32_t index0, index1;

        std::vector<uint32_t> indices;

        float distance;

        sf::VertexArray line;

        sf::ConvexShape Line;

    };

    struct planet {
    
        sf::Vector2f center;

        float radius;

    };

    struct border {

        sf::Vector2f center;

        float width;

        float length;

        bool is_horizontal;

    };

    struct soft_body {

        struct vertex {
        
            uint32_t index;

            sf::Vector2f  position;

        };

        sf::ConvexShape shape;

        sf::VertexArray inner_diagonals_of_shape;

        std::vector<vertex> vertices;

        sf::Vector2f center;

        std::vector<sf::Vector2f> vectors_from_center;

    };

    //struct engine {
        
        std::vector<point_mass> points;

        std::vector<sf::CircleShape> colliders;

        std::vector<distance_constraint> constraints;

        std::vector<soft_body> soft_bodies;

        std::vector<sf::ConvexShape> soft_bodies_shapes;

        std::vector<int> all_soft_bodies_indices;

        std::vector<sf::ConvexShape> Lines;

        std::vector<sf::VertexArray> lines;

        sf::Vector2f gravity{ 0, 1000};

        sf::Vector2f friction{ 0.5, 0.5 };

        sf::Vector2f elasticity{ 0.5, 0.5 };//1 elasticity means the object retains full velocity after bouncing

        float spring_force = 800;//increasing the number makes the spring/string way less mellow, but would basically throw the force back at you like a ballista 
                                 //increase it to make the object more stiff and less jello like. At 100 it very mello, 300 is jello, 800 is nice and springy - jello, 
                                 //5K is very springy making the the constraint between the points more firm, 10K is nice and crispy, 30k is very very crispy. More
                                 //than this will break

        float spring_damping = 10;//increasing dampening lessens the vibration from the spring force on the objects. BEST AT 2.5 / 5 / 10
                                  //PRO TIP: LEAVE IT AT 10 AND ONLY CHANGE THE SPRING_FORCE

        void create_collider(float radius, float x, float y, sf::Vector2f velocity, sf::Color color, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders);

        void create_colliders_with_constraints(float radius1, float x1, float y1, sf::Vector2f velocity1, float radius2, float x2, float y2, sf::Vector2f velocity2, float distance, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders, std::vector<distance_constraint>& constraints, std::vector<sf::ConvexShape>& Lines);

        Physics::soft_body create_soft_body_automatic(Physics& physics, std::vector<sf::Vector2f>& vertices_positions, sf::Vector2f velocity, float radius, sf::Color const& color, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders);

        void update_gravity_and_positions(float dt, std::vector<point_mass>& points);

        void update(float dt, planet const& myPlanet, std::vector<point_mass>& points);

        void update(float dt, border const& rect, std::vector<point_mass>& points);

        void update_planets(float dt, std::vector<planet> const& planets, std::vector<point_mass>& points);

        void update_borders(float dt, std::vector<border> const& borders, std::vector<point_mass>& points);

        void update_colliders(float dt, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders);

        void update_soft_bodies(float dt, std::vector<point_mass>& points);

        void update_constraints(float dt, std::vector<point_mass>& points, std::vector<distance_constraint>& constraints);

        void UPDATE(float dt, std::vector<planet> const& planets, std::vector<border> const& borders, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders, std::vector<distance_constraint>& constraints);////    

    //};

    void setWindow(sf::RenderWindow* window);

    void setFluids(std::shared_ptr<Fluids> fluids);

    sf::Vector2f multiplyVectors(sf::Vector2f const& A, sf::Vector2f const& B);

    float length(const sf::Vector2f& v);

    float dot(sf::Vector2f A, sf::Vector2f B);

    float cross(sf::Vector2f A, sf::Vector2f B);

    sf::Vector2f rotate(sf::Vector2f const& vector, float const& angle);

    void create_border(bool orientation, float length, float width, float x, float y);

    void create_planet(float radius, float x, float y);

    collision find_collision(sf::Vector2f const& position);

    collision find_collision(sf::Vector2f const& position, planet const& planet);

    collision find_collision(sf::Vector2f const& position, border const& rect);

    collision find_collision(sf::Vector2f const& position, std::vector<planet> const& planets);

    collision find_collision(sf::Vector2f const& position, std::vector<border> const& borders);

    collision find_collision(const point_mass& p1, const point_mass& p2, float radius1, float radius2);

    void render_circles();

    void render_rects();

    void render_soft_bodies(std::vector<point_mass>& points);

    void render_colliders(std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders);

    void render_constraints(std::vector<point_mass>& points, std::vector<distance_constraint>& constraints, std::vector<sf::ConvexShape>& Lines);

    //void render_lines();

    void RENDER(std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders, std::vector<distance_constraint>& constraints, std::vector<sf::ConvexShape>& Lines);

    void collider_control(std::vector<point_mass>& Points, std::vector<sf::CircleShape>& Colliders);

    void collider_control_thread(std::vector<point_mass>& Points, std::vector<sf::CircleShape>& Colliders);

    void start_collider_control_thread(std::vector<point_mass>& Points, std::vector<sf::CircleShape>& Colliders);

    void TEST(sf::RenderWindow* window, Physics& physics, bool& start);


    sf::RenderWindow* window;

    std::vector<border> borders;

    std::vector<sf::RectangleShape> rects;

    std::vector<planet> planets;

    std::vector<sf::CircleShape> circles;

    std::atomic<bool> Held = false;

    std::atomic<bool> wasHeld = false;

    std::atomic<bool> Valid = false;

    std::atomic<bool> index_caught = false;

    std::atomic<int> index;

    std::atomic<int> point_index;

    //these 2 vectors are used in the Fluids class, however they are here created due to the problem of having to forward delcare their type (nested structure in a class)
    //in the Fluids class
    std::vector<point_mass> blood_points;

    std::vector<sf::CircleShape> blood_colliders;

    private:

        std::shared_ptr<Fluids> fluids;

};

