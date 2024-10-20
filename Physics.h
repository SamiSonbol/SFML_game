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

        float friction_coefficient;
        float youngs_modulus;//young's modulus is the stiffness of the material, the more it increases the less elastic the material becomes
        float damping_ratio;

        float compute_spring_constant(const float& cross_section_area, const float& length_of_spring_at_rest);
        float compute_spring_constant(const float& length_of_spring_at_rest);

        float compute_damping_coefficient(const float& spring_constant, const float& mass);

        Material(const float& friction_coefficient, const float& youngs_modulus, const float& damping_ratio);

        static float to_GPa(const float& Pa);
        static float to_Pa(const float& GPa);

        static Material iron();
        static Material wood();
        static Material rubber();
        static Material jello();

    };

    struct Collision {

        vec3 normal = vec3(0.0f, 0.0f, 0.0f);

        float depth = -std::numeric_limits<float>::infinity();

    };

    class point_mass {
    
    public:

        float mass;
        float radius;

        vec3 position;      
        vec3 velocity;
        vec3 net_force;

        uint32_t index;

        Physics::Collision find_collision(const point_mass& point);
        void collide(point_mass& point, const float& point_elasticity, const float& elasticity);

        vec3 compute_spring_force(const vec3& anchor_pos, const float& length_of_spring_at_rest, const float& spring_constant);
        vec3 compute_weight_force(const vec3& gravity);
        vec3 compute_friction_force(const point_mass& point, const float& friction_coefficient, const vec3& gravity);  
        vec3 compute_damping_force(const float& damping_coefficient, const float& max_damping_force);
        
        void add_force(const vec3& force, const float& delta_time);
        void apply_forces(const float& delta_time);
        void reset_forces();
     
        point_mass(const vec3& position, const vec3& velocity, const float& radius = 1.0f, const float& mass = 0.1f);

    };

    class Body {

    public:

        Material material;

        sf::ConvexShape frame;
        vec3 center;

        std::vector<point_mass> points;
        std::vector<float> center_to_point_length;//this vector holds the length between each point and its neighbor at initial state 

        uint32_t index;

        float mass;

        void print_debug_information();

        void update_center();
        vec3 get_bounds();

        vec3 get_position();
        void set_position(const vec3& position);
        void rotate(const vec3& rotation_vector);

        Physics::Collision find_collision(const Body& body);

        //WARNING: this function should not be used unless you want to have mass_points of different masses on the same object. Default is that each point's mass would get its value based off the
        //inputed Body's mass divided by total number of points => hence the Body's mass needs to be calculated instead of inputed if you want to have unique or diff mass for points
        //WARNING: just use default standard mass implementation since its easier; 
        float calculate_total_mass();

        void update(const float& delta_time, const vec3& gravity);

        void collide(const float& delta_time, Body& body, const vec3& gravity);

        void render(sf::RenderWindow* window);

        Body(const std::vector<vec3>& positions, const Material& material, const float& mass, const float& radius = 1.0f, const vec3& velocity = {0.0f, 0.0f, 0.0f});

    };
        
    std::vector<Body> Bodies;

    std::atomic<bool> mouse_held = false;
    std::atomic<bool> mouse_was_held = false;
    std::atomic<bool> new_selection_valid = true;

    std::atomic<int> caught_body_index;
    std::atomic<int> caught_point_index;
    std::vector<vec3> mouse_drawn_body_positions;

    void UPDATE(const float& delta_time, const vec3& gravity);
    void RENDER(sf::RenderWindow* window);

    void collider_control(sf::RenderWindow* window);
    void TEST(sf::RenderWindow* window, vec3& gravity, bool& start);

    static void launch(Body& body, const vec3& velocity) {

        for (auto& point : body.points) {

            point.velocity = velocity;

        };

    };

};

