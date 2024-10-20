#include "physics.h"

Physics::Material::Material(const float& friction_coefficient, const float& youngs_modulus, const float& damping_ratio) :

    friction_coefficient(friction_coefficient),
    youngs_modulus(youngs_modulus),
    damping_ratio(damping_ratio) {

};

float Physics::Material::compute_spring_constant(const float& cross_section_area, const float& length_of_spring_at_rest) {//3D objects

    return (cross_section_area * this->youngs_modulus) / length_of_spring_at_rest;
    

};

float Physics::Material::compute_spring_constant(const float& length_of_spring_at_rest) {//2D objects

    return this->youngs_modulus / length_of_spring_at_rest;

};

float Physics::Material::compute_damping_coefficient(const float& spring_constant, const float& mass) {

    return 2 * this->damping_ratio * std::sqrt(spring_constant * mass);

};

float Physics::Material::to_GPa(const float& Pa) {

    return Pa / 1000000000.0f;

};

float Physics::Material::to_Pa(const float& GPa) {

    return GPa * 1000000000.0f;

};

Physics::Material Physics::Material::iron() {

    return Material(0.6f, (210.0f), 0.05f);

};

Physics::Material Physics::Material::wood() {

    return Material(0.6f, (16.0f), 0.1f);

};

Physics::Material Physics::Material::rubber() {

    return Material(0.8f, (1.0f), 0.5f);

};

Physics::Material Physics::Material::jello() {

    return Material(0.1f, (0.01f), 0.7f);

};

Physics::Collision Physics::point_mass::find_collision(const point_mass& point) {

    vec3 delta = this->position - point.position;

    float distance = delta.magnitude();

    float EPSILON = 1e-6;

    if (distance < EPSILON) {

        return Collision{ {0,0}, 0 };

    }
    else {

        float depth = this->radius + point.radius - distance;

        return Collision{ delta.normalize(), depth};

    };

};

void Physics::point_mass::collide(point_mass& point, const float& point_elasticity, const float& elasticity) {

    if (elasticity == 0.0f && point_elasticity == 0.0f) {//perfectly inelastic collision

        vec3 momentum = (point.velocity * point.mass) + (this->velocity * this->mass);
        this->velocity = momentum / (point.mass + this->mass);
        point.velocity = this->velocity;

    }
    else if (elasticity == 1.0f && point_elasticity == 1.0f) {//perfectly elastic collision. Moreover elasticity cannot actually be physically more than 1.0f, but to avoid edge cases we used >= 1.0f

        vec3 momentum = this->velocity * this->mass;
        vec3 point_momentum = point.velocity * point.mass;
        float total_mass = this->mass + point.mass;

        //formula: final velocity1 = (mass1 - mass2)*initial velocity1 + 2(initial velocity2 * mass2)
        float relative_mass = this->mass - point.mass;
        vec3 coefficient = (this->velocity * relative_mass) + (point_momentum * 2);
        this->velocity = coefficient / total_mass;

        //formula: final velocity2 = (mass2 - mass1)*initial velocity2 + 2(initial velocity1 * mass1)
        relative_mass = point.mass - this->mass;
        coefficient = (point.velocity * relative_mass) + (momentum * 2);
        point.velocity = coefficient / total_mass;

    }
    else {//general elastic collision => either both elasticities are between ]0,1[  or  one is 0.0f and the other is 1.0f. Moreover, the formula is the almost same as the perfectly elastic collision 

        //vec3 momentum = this->velocity * this->mass;
        //vec3 point_momentum = point.velocity * point.mass;
        //float total_mass = this->mass + point.mass;

        ////formula: final velocity1 = (mass1 - (elasticity2*mass2))*initial velocity1 + 2(initial velocity2 * mass2)
        //float relative_mass = this->mass - (point_elasticity * point.mass);
        //vec3 coefficient = (this->velocity * relative_mass) + (point_momentum * 2);
        //this->velocity = coefficient / total_mass;

        ////formula: final velocity2 = (mass2 - (elasticity1*mass1))*initial velocity2 + 2(initial velocity1 * mass1)
        //relative_mass = point.mass - (elasticity * this->mass);
        //coefficient = (point.velocity * relative_mass) + (momentum * 2);
        //point.velocity = coefficient / total_mass;



        //float total_mass = this->mass + point.mass;
        //float E = point_elasticity;
        //float coof = point_elasticity * point.mass;
        //float relative_mass = this->mass - coof; 
        //vec3 momentum_coof = (this->velocity * coof) * this->mass;
        //vec3 coefficient = (this->velocity * relative_mass) + momentum_coof;
        //this->velocity = coefficient / total_mass;

        //E = elasticity;
        //coof = elasticity * this->mass;
        //relative_mass = point.mass - coof;
        //momentum_coof = (point.velocity * coof) * point.mass;
        //coefficient = (point.velocity * relative_mass) + momentum_coof;
        //point.velocity = coefficient / total_mass;

        float e = (elasticity + point_elasticity) / 2.0f;
        float e2 = point_elasticity;
        float e1 = elasticity;

        float mass1 = this->mass;
        float mass2 = point.mass;
        float total_mass = mass1 + mass2;

        vec3 v1 = this->velocity;
        vec3 v2 = point.velocity;
        vec3 momentum1 = v1 * mass1;
        vec3 momentum2 = v2 * mass2;

        vec3 collisionNormal = find_collision(point).normal;
        float relativeVelocity = (v2 - v1).dot(collisionNormal);
        if (relativeVelocity > 0.0f) {

            return;

        };
        
        //final velocities using the elastic collision formulas
       /* this->velocity.x = ((mass1 - mass2 * e1) * v1.x + (2 * mass2 * (1 + e1) * v2.x)) / total_mass;
        this->velocity.y = ((mass1 - mass2 * e1) * v1.y + (2 * mass2 * (1 + e1) * v2.y)) / total_mass;
        this->velocity.z = ((mass1 - mass2 * e1) * v1.z + (2 * mass2 * (1 + e1) * v2.z)) / total_mass;

        point.velocity.x = ((mass2 - mass1 * e2) * v2.x + (2 * mass1 * (1 + e2) * v1.x)) / total_mass;      
        point.velocity.y = ((mass2 - mass1 * e2) * v2.y + (2 * mass1 * (1 + e2) * v1.y)) / total_mass;    
        point.velocity.z = ((mass2 - mass1 * e2) * v2.z + (2 * mass1 * (1 + e2) * v1.z)) / total_mass;*/

        
        float impulse = -(1 + e1) * relativeVelocity / total_mass;
        vec3 impulseVector = collisionNormal * impulse;
        this->velocity -= impulseVector * (mass2 / total_mass);

        impulse = -(1 + e2) * relativeVelocity / total_mass;
        impulseVector = collisionNormal * impulse;
        point.velocity += impulseVector * (mass1 / total_mass);

    };

};

vec3 Physics::point_mass::compute_spring_force(const vec3& anchor_pos, const float& length_of_spring_at_rest, const float& spring_constant) {

    //getting the vector spanning from our anchor to our current position => this is the current vector resembeling our spring. NOTE: this vector contains both direction and magnitude
    vec3 anchor_to_point = (this->position - anchor_pos);

    //calculating the difference between our current spring length and the length of said spring at rest
    float length_difference = anchor_to_point.magnitude() - length_of_spring_at_rest;
  
    //normalizing our current spring vector inorder to get purely its direction and using this formula: F = direction * -spring_constant * length_difference => we get the spring force
    return anchor_to_point.normalize() * (-spring_constant * length_difference);

};

vec3 Physics::point_mass::compute_weight_force(const vec3& gravity) {

    return gravity * this->mass;

};

vec3 Physics::point_mass::compute_friction_force(const point_mass& point, const float& friction_coefficient, const vec3& gravity) {

    //friction pushes our point against the direction its trying to go to => friction direction = -direction of velocity
    vec3 friction_direction = this->velocity.normalize() * -1;

    //inorder to get the angle of the slope of which friction occurs between our points we need to find the vector between them and use the formula arctan(vec.z/vec.magnitude);
    vec3 this_to_point = point.position - this->position;
    float horizontal_distance = std::sqrt(this_to_point.x * this_to_point.x + this_to_point.y * this_to_point.y);
    float angle_of_slope = atan2(this_to_point.z, horizontal_distance);
    vec3 normal_force = gravity * this->mass * cos(angle_of_slope);//normal force = weight * cos(angle of slope)

    //using this formula: friction force = friction_direction * friction coefficient * normal force
    vec3 friction_force = friction_direction * normal_force * friction_coefficient;
    vec3 weight_down_slope = (gravity * this->mass).magnitude() * sin(angle_of_slope);//weight_down_slope returns how does the object rotate down on our slope due to gravity
    return weight_down_slope - friction_force;

};

vec3 Physics::point_mass::compute_damping_force(const float& damping_coefficient, const float& max_damping_force) {

    vec3 damping_force = this->velocity * -damping_coefficient;

    if (damping_force.magnitude() > max_damping_force) {

        damping_force = damping_force.normalize() * max_damping_force;

    };

    return damping_force;

};

void Physics::point_mass::add_force(const vec3& force, const float& delta_time) {

    this->net_force += force * delta_time;

};

void Physics::point_mass::apply_forces(const float& delta_time) {

    vec3 acceleration = this->net_force / this->mass;
    this->velocity += acceleration * delta_time;
    this->position += this->velocity * delta_time;

};

void Physics::point_mass::reset_forces() {

    this->net_force = { 0, 0, 0 };//the z-coord value here is 0 and not 1 since net_force vector resembles physics forces and their values, not graphical values or directions.  

};

Physics::point_mass::point_mass(const vec3& position, const vec3& velocity, const float& radius, const float& mass) : 
    
    position(position), velocity(velocity), mass(mass), radius(radius) {

};

void Physics::Body::print_debug_information() {

     for (auto& point : this->points) {

        std::cout << "initial position of vertex " << point.index << " is " << point.position.x << ", " << point.position.y << std::endl;

    };

    int i = 0;
    for (auto& p : this->points) {

        std::cout << "initial position of mass point " << i << " is " << p.position.x << ", " << p.position.y << std::endl;
        i++;

    }

    for (int i = 0; i < this->frame.getPointCount(); i++) {

        std::cout << "initial position of soft body point " << i << " is " << this->frame.getPoint(i).x << ", " << this->frame.getPoint(i).y << std::endl;

    }

    std::cout << "point_mass count: " << this->points.size() << std::endl;
    std::cout << "convex shape points count: " << this->frame.getPointCount() << std::endl;
    std::cout << "number of center-to-point vectors: " << this->center_to_point_length.size() << std::endl;

};

void Physics::Body::update_center() {

    this->center = { 0, 0, 1 };
    for (auto const& point : this->points) {

        this->center += points[point.index].position;

    };

    this->center /= (float)this->points.size();

};

vec3 Physics::Body::get_bounds() {

    vec3 start_pos = this->points[0].position;
    vec3 end_pos = start_pos;
    for (auto& point : this->points) {

        if (point.position.x < start_pos.x) {

            start_pos.x = point.position.x;

        };
        if (point.position.x > end_pos.x) {

            end_pos.x = point.position.x;

        };

        if (point.position.y < start_pos.y) {

            start_pos.y = point.position.y;

        };
        if (point.position.y > end_pos.y) {

            end_pos.y = point.position.y;

        };

        if (point.position.z < start_pos.z) {

            start_pos.z = point.position.z;

        };
        if (point.position.z > end_pos.z) {

            end_pos.z = point.position.z;

        };

    };

    vec3 distance = end_pos - start_pos;
    if (distance.x < 0) {

        distance.x *= -1;

    };
    if (distance.y < 0) {

        distance.y *= -1;

    };
    if (distance.z < 0) {

        distance.z *= -1;

    };

    return distance;//x is length, y is width, z is depth

};

vec3 Physics::Body::get_position() {

    return vec3(this->center.x, this->center.y);

};

void Physics::Body::set_position(const vec3& position) {

    vec3 center_difference(position - this->center);
    mat4 translation_matrix = create_translation_matrix(center_difference);
    for (auto& point : this->points) {

        vec4 new_pos = translation_matrix * point.position;
        point.position = { new_pos.x, new_pos.y, new_pos.z };
        this->frame.setPoint(point.index, { point.position.x, point.position.y });
        
    };

    this->update_center();

};

void Physics::Body::rotate(const vec3& rotation_vector) {

    vec3 original_pos = this->get_position();
    this->set_position({ 0, 0, 1 });//translating to a different coord so we can return to our original coords after rotation => hence having done a rotation around our object's origin

    mat4 rotation_matrix = create_rotation_matrix(rotation_vector.x, rotation_vector.y, rotation_vector.z);
    for (auto& point : this->points) {

        vec4 new_pos = rotation_matrix * point.position;
        point.position = { new_pos.x, new_pos.y, new_pos.z };
        this->frame.setPoint(point.index, { point.position.x, point.position.y });

    };

    this->set_position(original_pos);
    this->update_center();

};

float Physics::Body::calculate_total_mass() {

    float mass = 0.0f;
    for (const auto& point : this->points) {

        mass += point.mass;

    };

    return mass;

};

Physics::Body::Body(const std::vector<vec3>& positions, const Material& material, const float& mass, const float& radius, const vec3& velocity) : material(material), mass(mass) {

    int total_points = 0;

    float space_manager = radius * 3.55f;// * 2.25 provides best simulation but causes big instabilities in various shapes due to overcrowding colliders/vertices
                                        // * 3.55 provides the best stability

    std::vector<int> sizes;

    for (int i = 0; i < positions.size(); ++i) {

        int next_index = (i + 1) % positions.size();

        //distance between this position and the next
        float distance = (positions[next_index] - positions[i]).magnitude();

        int n_points = distance / space_manager;

        total_points += n_points;

        sizes.emplace_back(std::move(n_points));

    };

    this->frame.setPointCount(total_points);

    float mass_per_point = this->mass / total_points;

    for (int i = 0; i < positions.size(); i++) {

        int next_index = (i + 1) % positions.size();

        //std::cout << "Origin point " << i << " at coordinates: " << positions[i].x << ", " << positions[i].y << " has an edge with the next point; that consists of " << sizes[i] << " vertices/colliders\n" << std::endl;

        for (int j = 0; j < sizes[i]; j++) {

            if (j == 0) {

                this->points.emplace_back(positions[i], velocity, radius, mass_per_point);
                this->points[this->points.size() - 1].index = this->points.size() - 1;
                this->frame.setPoint(this->points.size() - 1, sf::Vector2f(this->points[this->points.size() - 1].position.x, this->points[this->points.size() - 1].position.y));

            }

            else {//VERY IMPORTANT NOTE: WE ADD OR SUBTRACT THE RADIUS FROM THE NEXTPOSITION SINCE THE POSITION REFERS TO THE CENTER OF THE COLLIDER AND NOT THE
                //USUAL ORIGIN POINT OF THE DRAWN CIRCLE. WE ALSO ADD OR SUBTRACT THE SPACEMANAGER FROM THE CURRENT COLLIDER POSITION SINCE ITS WHERE THE NEW
                //POINT POSITION SHOULD BE IF WE INDEED CREATED THE NEW COLLIDER

                point_mass point = point_mass(this->points[this->points.size() - 1].position, velocity, radius, mass_per_point);

                if (positions[next_index].x - radius > point.position.x + space_manager) {

                    point.position.x += space_manager;

                }
                else if (positions[next_index].x + radius < point.position.x - space_manager) {

                    point.position.x -= space_manager;

                };

                if (positions[next_index].y + radius < point.position.y - space_manager) {

                    point.position.y -= space_manager;

                }
                else if (positions[next_index].y - radius > point.position.y + space_manager) {

                    point.position.y += space_manager;

                };

              // if (point.position == positions[next_index]) {
                   // std::cout << "alive" << std::endl;
                                       
                    this->points.emplace_back(std::move(point));
                    this->points[this->points.size() - 1].index = this->points.size() - 1;
                    this->frame.setPoint(this->points.size() - 1, sf::Vector2f(this->points[this->points.size() - 1].position.x, this->points[this->points.size() - 1].position.y));

               //};

            }

        };

    };

    vec3 center_to_point;
    update_center();
    for (int i = 0; i < this->points.size(); ++i) {

        center_to_point = this->points[i].position - this->center;
        this->center_to_point_length.emplace_back(center_to_point.magnitude());

    };
    
    print_debug_information();

};

Physics::Collision Physics::Body::find_collision(const Body& body) {

    for (auto& point : this->points) {

        for (auto& other_point : body.points) {

            if (point.find_collision(other_point).depth > 0.0f) {

                return point.find_collision(other_point);

            };

        };

    };

};

void Physics::Body::collide(const float& delta_time, Body& body, const vec3& gravity) {

    //if (this->find_collision(body).depth > 0.0f) {

        vec3 friction;
        for (auto& point : this->points) {

            for (auto& other_point : body.points) {

                if (point.find_collision(other_point).depth > 0.0f) {
                    
                    friction = point.compute_friction_force(other_point, this->material.friction_coefficient, gravity);
                    point.add_force(friction, delta_time);

                    point.collide(other_point, body.material.youngs_modulus, this->material.youngs_modulus);
                    
                };

            };

        };

    //};

};

void Physics::Body::update(const float& delta_time, const vec3& gravity) {

    update_center();

    float cross_section_area;
    float extension;
    float spring_constant;
    
    vec3 spring_force;
    vec3 damping;

    vec3 weight;
    vec3 acceleration; 

    for (auto& point : this->points) {
      
        weight = point.compute_weight_force(gravity);
        point.add_force(weight, delta_time);
        
        spring_constant = this->material.compute_spring_constant(this->center_to_point_length[point.index]);  
        std::cout << "sring_constant: " << spring_constant << std::endl;
        spring_force = point.compute_spring_force(this->center, this->center_to_point_length[point.index], spring_constant);  
        point.add_force(spring_force, delta_time);
          
        damping = point.compute_damping_force(this->material.compute_damping_coefficient(spring_constant, point.mass), spring_force.magnitude() * 0.1f);
        point.add_force(damping, delta_time);

        point.apply_forces(delta_time);
        this->frame.setPoint(point.index, sf::Vector2f(point.position.x, point.position.y));

        point.reset_forces();

    };

};

void Physics::Body::render(sf::RenderWindow* window) {

    window->draw(this->frame);

};

void Physics::RENDER(sf::RenderWindow* window) {

    for (auto& body : this->Bodies) {

        body.render(window);

    };

};

void Physics::UPDATE(const float& delta_time, const vec3& gravity) {

    for (auto& body : this->Bodies) {

        for (auto& other_body : this->Bodies) {

            if (body.index != other_body.index) {

                body.collide(delta_time, other_body, gravity);

            };

        };  

        body.update(delta_time, gravity);

    };

};

void Physics::collider_control(sf::RenderWindow* window) {
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        if (!mouse_held.load()) {

            mouse_held.store(true);

        };

    }

    else {

        if (mouse_held.load()) {

            mouse_held.store(false);
      
        };

    };

    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

    sf::Vector2i prev_mousePos;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && mouse_was_held.load() && new_selection_valid.load()) {

        this->mouse_drawn_body_positions.emplace_back(mousePos.x, mousePos.y);

    };

    if (mouse_held.load() && !this->Bodies.empty()) {

        mouse_was_held.store(true);

        if (mousePos.x >= 0 && mousePos.x <= sf::VideoMode::getDesktopMode().width && mousePos.y >= 0 && mousePos.y <= sf::VideoMode::getDesktopMode().height) {

            if (!new_selection_valid.load()) {

                this->Bodies[caught_body_index].points[caught_point_index].position.x = mousePos.x;
                this->Bodies[caught_body_index].points[caught_point_index].position.y = mousePos.y;
                this->Bodies[caught_body_index].points[caught_point_index].velocity = { 0, 0 };

                prev_mousePos = mousePos;

            }
            else {

                int body_count = 0;
                for (auto& body : this->Bodies) {
                    
                    if (mousePos.x >= body.frame.getPosition().x - body.frame.getGlobalBounds().width &&
                        mousePos.x <= body.frame.getPosition().x + body.frame.getGlobalBounds().width &&
                        mousePos.y >= body.frame.getPosition().y - body.frame.getGlobalBounds().height &&
                        mousePos.y <= body.frame.getPosition().y + body.frame.getGlobalBounds().height) continue;

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

                        vec3 v(0, 100, 0);
                        launch(body, v);

                    }
                    else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {

                        //since we are in 2D the correct rotation axis is Z
                        body.rotate({0, 0, 30});                       

                    };

                        for (auto& point : body.points) {

                            if (mousePos.x >= point.position.x - point.radius &&
                                mousePos.x <= point.position.x + point.radius &&
                                mousePos.y >= point.position.y - point.radius &&
                                mousePos.y <= point.position.y + point.radius) {

                                new_selection_valid.store(false);
                                caught_body_index.store(body_count);
                                caught_point_index.store(point.index);

                                break;

                            };

                        };

                        body_count++;//incrementing

                };

            };

        };

    };

    if (!mouse_held.load() && mouse_was_held.load()) {
        
        mouse_was_held.store(false);

        new_selection_valid.store(true);

        if (!this->mouse_drawn_body_positions.empty()) {

            
            Body body(this->mouse_drawn_body_positions, Material::jello(), 400, 2);
            body.frame.setFillColor(sf::Color(0, 250, 0, 250));
            body.index = this->Bodies.size() - 1;
            this->Bodies.emplace_back(std::move(body));

            this->mouse_drawn_body_positions.clear();
        };
        
    };

};

void Physics::TEST(sf::RenderWindow* window, vec3& gravity, bool& start) {

    bool create = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

        sf::Vector2i mouse_pos = sf::Mouse::getPosition();

        sf::Vector2f velocity = sf::Vector2f(0, 0);

        float r = 10;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {

            velocity.x = 100;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {

            velocity.x = -100;

            create = true;

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

            velocity.y = -100;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

            velocity.y = 100;

            create = true;

        }

        if (start && create) {

            sf::CircleShape circle(r * 2);
            circle.setPosition(sf::Vector2f(mouse_pos));
            std::vector<vec3> vec;
            for (int i = 0; i < circle.getPointCount(); ++i) {

                vec.emplace_back(circle.getPoint(i).x + mouse_pos.x, circle.getPoint(i).y + mouse_pos.y, 0);

            };
         
            //creating a body inside the vector
            this->Bodies.emplace_back(vec, Material::rubber(), 62, 1, vec3(velocity.x, velocity.y));
            this->Bodies[this->Bodies.size() - 1].frame.setFillColor(sf::Color(0, 0, 250, 250));
            this->Bodies[this->Bodies.size() - 1].index = this->Bodies.size() - 1;

            start = false;

            create = false;

        };

    };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {

        sf::Vector2i mouse_pos = sf::Mouse::getPosition();

        sf::Vector2f velocity = sf::Vector2f(0, 0);

        std::vector<vec3> rectangleVertices = {
        
            vec3(mouse_pos.x, mouse_pos.y),

            vec3(mouse_pos.x + 50, mouse_pos.y),

            vec3(mouse_pos.x + 100, mouse_pos.y + 50),

            vec3(mouse_pos.x + 100, mouse_pos.y + 100),

            vec3(mouse_pos.x + 50, mouse_pos.y + 150),

            vec3(mouse_pos.x, mouse_pos.y + 150),

            vec3(mouse_pos.x - 50, mouse_pos.y + 100),

            vec3(mouse_pos.x - 50, mouse_pos.y + 50),
                                          
        };

        float r = 4;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {

            velocity.x = 300;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {

            velocity.x = -300;

            create = true;

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

            velocity.y = -300;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

            velocity.y = 300;

            create = true;

        }

        if (start && create) {

            this->Bodies.emplace_back(rectangleVertices, Material::iron(), 5000, 2, vec3(velocity.x, velocity.y));
            this->Bodies[this->Bodies.size() - 1].frame.setFillColor(sf::Color(1, 50, 90, 250));
            this->Bodies[this->Bodies.size() - 1].index = this->Bodies.size() - 1;
            
            start = false;

            create = false;

        }

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {

        gravity.y += 3;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

        gravity.x += 3;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {

        gravity.x -= 3;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {

        gravity.y -= 3;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {

        gravity.x = 0;

        gravity.y = 0;

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {

        start = true;

    }

};
