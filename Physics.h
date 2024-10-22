#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <thread>
#include <iostream>
#include "Math.h"

class Physics
{

public:

    struct Material {

    public:

        double friction_coefficient;
        double youngs_modulus;//young's modulus is the stiffness of the material, the more it increases the less elastic the material becomes
        double damping_ratio;

        double compute_spring_constant(const double& cross_section_area, const double& length_of_spring_at_rest);
        double compute_spring_constant(const double& length_of_spring_at_rest);

        double compute_damping_coefficient(const double& spring_constant, const double& mass);

        Material(const double& friction_coefficient, const double& youngs_modulus, const double& damping_ratio);

        static double Pa_to_MPa(const double& Pa);
        static double Pa_to_GPa(const double& Pa);
        static double MPa_to_Pa(const double& MPa);
        static double MPa_to_GPa(const double& MPa);
        static double GPa_to_Pa(const double& GPa);
        static double GPa_to_MPa(const double& GPa);

        static Material iron();
        static Material wood();
        static Material rubber();
        static Material jello();

    };

    struct Collision {

        vec3 normal = vec3(0.0f, 0.0f, 0.0f);

        double depth = -std::numeric_limits<double>::infinity();

    };

    struct Constraint {//index is the index of the point_mass that is connected to the current point_mass that holds this spring inside the same body.

        uint32_t index_1;
        uint32_t index_2;

        double length_at_rest;

        Constraint(const uint32_t& index_1, const uint32_t& index_2, const double& length_at_rest);

    };

    class point_mass {
    
    public:

        double mass;
        double radius;

        vec3 position;      
        vec3 velocity;
        vec3 net_force;

        uint32_t index;

        Physics::Collision find_collision(const point_mass& point);
        void collide(point_mass& point, const double& point_elasticity, const double& elasticity);

        vec3 compute_spring_force(const vec3& anchor_pos, const double& length_of_spring_at_rest, const double& spring_constant);
        vec3 compute_weight_force(const vec3& gravity);
        vec3 compute_friction_force(const point_mass& point, const double& friction_coefficient, const vec3& gravity);
        vec3 compute_damping_force(const double& damping_coefficient, const double& max_damping_force);
        
        void add_force(const vec3& force, const double& delta_time);
        void apply_forces(const double& delta_time);
        void reset_forces();
     
        point_mass(const vec3& position, const vec3& velocity, const double& radius = 1.0f, const double& mass = 0.1f);

    };

    class Body {

    public:

        sf::ConvexShape frame;
        Material material;
        vec3 center;

        std::vector<point_mass> points;
        std::vector<Constraint> constraints;
        std::vector<double> center_to_point_length;//this vector holds the length between each point and its neighbor at initial state 

        uint32_t index;

        double mass;

        void print_debug_information();

        void constrain_points(const point_mass& point_1, const point_mass& point_2);

        void update_center();
        vec3 get_bounds();

        vec3 get_position();
        void set_position(const vec3& position);
        void rotate(const vec3& rotation_vector);

        Physics::Collision find_collision(const Body& body);
        

        //WARNING: this function should not be used unless you want to have mass_points of different masses on the same object. Default is that each point's mass would get its value based off the
        //inputed Body's mass divided by total number of points => hence the Body's mass needs to be calculated instead of inputed if you want to have unique or diff mass for points
        //WARNING: just use default standard mass implementation since its easier; 
        double calculate_total_mass();

        void update(const double& delta_time, const vec3& gravity);

        void collide(const double& delta_time, Body& body, const vec3& gravity);

        void render(sf::RenderWindow* window);

        Body(const std::vector<vec3>& positions, const Material& material, const double& mass, const double& radius = 1.0f, const vec3& velocity = {0.0f, 0.0f, 0.0f});

    };
        
    std::vector<Body> Bodies;

    std::atomic<bool> mouse_held = false;
    std::atomic<bool> mouse_was_held = false;
    std::atomic<bool> new_selection_valid = true;

    std::atomic<int> caught_body_index;
    std::atomic<int> caught_point_index;
    std::vector<vec3> mouse_drawn_body_positions;

    void UPDATE(const double& delta_time, const vec3& gravity);
    void RENDER(sf::RenderWindow* window);

    void collider_control(sf::RenderWindow* window);
    void TEST(sf::RenderWindow* window, vec3& gravity, bool& start);

    static void launch(Body& body, const vec3& velocity) {

        for (auto& point : body.points) {

            point.velocity = velocity;

        };

    };

    static std::vector<sf::CircleShape> return_skeleton(const Body& body) {

        std::vector<sf::CircleShape> circles;
        circles.reserve(body.points.size());
        for (auto& point : body.points) {

            sf::CircleShape C(point.radius);
            C.setFillColor(sf::Color::Black);
            C.setPosition(point.position.x, point.position.y);
            circles.emplace_back(std::move(C));

        };
        return circles;

    };

    static void draw_skeleton(sf::RenderWindow* window, const std::vector<sf::CircleShape>& circles) {

        for (auto& c : circles) {

            window->draw(c);

        };

    };

};

