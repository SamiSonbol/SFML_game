#include "physics.h"

Physics::Material::Material(const double& friction_coefficient, const double& youngs_modulus, const double& damping_ratio) :

    friction_coefficient(friction_coefficient),
    youngs_modulus(youngs_modulus),
    damping_ratio(damping_ratio) {

};

double Physics::Material::compute_spring_constant(const double& cross_section_area, const double& length_of_spring_at_rest) {//3D objects

    return (cross_section_area * this->youngs_modulus) / length_of_spring_at_rest;
    

};

double Physics::Material::compute_spring_constant(const double& length_of_spring_at_rest) {//2D objects

    return this->youngs_modulus / length_of_spring_at_rest;

};

double Physics::Material::compute_damping_coefficient(const double& spring_constant, const double& mass) {

    return 2 * this->damping_ratio * std::sqrt(spring_constant * mass);

};

double Physics::Material::GPa_to_Pa(const double& GPa) {

    return GPa * 1000000000.0f;

};

double Physics::Material::Pa_to_GPa(const double& Pa) {

    return Pa / 1000000000.0f;

};

double Physics::Material::MPa_to_Pa(const double& MPa) {

    return MPa * 1000000.0f;

};

double Physics::Material::Pa_to_MPa(const double& Pa) {

    return Pa / 1000000.0f;

};

double Physics::Material::GPa_to_MPa(const double& GPa) {

    return GPa * 1000.0f;

};

double Physics::Material::MPa_to_GPa(const double& MPa) {

    return MPa / 1000.0f;

};


Physics::Material Physics::Material::iron() {

    return Material(0.6f, MPa_to_GPa(210.0f), 0.05f);

};

Physics::Material Physics::Material::wood() {

    return Material(0.6f, GPa_to_Pa(16.0f), 0.1f);

};

Physics::Material Physics::Material::rubber() {

    return Material(0.8f, MPa_to_Pa(1.0f), 0.5f);

};

Physics::Material Physics::Material::jello() {

    return Material(0.1f, MPa_to_Pa(0.01f), (0.7f));

};

Physics::Constraint::Constraint(const uint32_t& index_1, const uint32_t& index_2, const double& length_at_rest) : index_1(index_1), index_2(index_2), length_at_rest(length_at_rest) {};

Physics::Collision Physics::point_mass::find_collision(const point_mass& point) {

    vec3 delta = this->position - point.position;

    double distance = delta.magnitude();

    double EPSILON = 1e-6;

    if (distance < EPSILON) {

        return Collision{ {0,0}, 0 };

    }
    else {

        double depth = this->radius + point.radius - distance;

        return Collision{ delta.normalize(), depth};

    };

};

void Physics::point_mass::collide(point_mass& point, const double& point_elasticity, const double& elasticity) {

    if (elasticity == 0.0f && point_elasticity == 0.0f) {//perfectly inelastic collision

        vec3 momentum = (point.velocity * point.mass) + (this->velocity * this->mass);
        this->velocity = momentum / (point.mass + this->mass);
        point.velocity = this->velocity;

    }
    else if (elasticity == 1.0f && point_elasticity == 1.0f) {//perfectly elastic collision. Moreover elasticity cannot actually be physically more than 1.0f, but to avoid edge cases we used >= 1.0f  
        
        double total_mass = this->mass + point.mass;

        //formula: final velocity1 = (mass1 - mass2)*initial velocity1 + 2(initial velocity2 * mass2)
        vec3 coof = this->velocity * this->mass - this->velocity * point.mass;
        vec3 coefficient = (point.velocity * point.mass) * 2;
        this->velocity = (coof + coefficient) / total_mass;

        //formula: final velocity2 = (mass2 - mass1)*initial velocity2 + 2(initial velocity1 * mass1)
        coof = point.velocity * point.mass - point.velocity * this->mass;
        coefficient = (this->velocity * this->mass) * 2;
        point.velocity = (coof + coefficient) / total_mass;

    }
    else {//general elastic collision => either both elasticities are between ]0,1[  or  one is 0.0f and the other is 1.0f. Moreover, the formula is the almost same as the perfectly elastic collision 

        vec3 collisionNormal = find_collision(point).normal;
        double relativeVelocity = (point.velocity - this->velocity).dot(collisionNormal);
        if (relativeVelocity > 0.0f) {

            return;

        };

        //vec3 momentum = this->velocity * this->mass;
        //vec3 point_momentum = point.velocity * point.mass;
        //double total_mass = this->mass + point.mass;

        ////formula: final velocity1 = (mass1 - (elasticity2*mass2))*initial velocity1 + 2(initial velocity2 * mass2)
        //double relative_mass = this->mass - (point_elasticity * point.mass);
        //vec3 coefficient = (this->velocity * relative_mass) + (point_momentum * 2);
        //this->velocity = coefficient / total_mass;

        ////formula: final velocity2 = (mass2 - (elasticity1*mass1))*initial velocity2 + 2(initial velocity1 * mass1)
        //relative_mass = point.mass - (elasticity * this->mass);
        //coefficient = (point.velocity * relative_mass) + (momentum * 2);
        //point.velocity = coefficient / total_mass;



       /* double total_mass = this->mass + point.mass;
        double E = point_elasticity;
        double coof = point_elasticity * point.mass;
        double relative_mass = this->mass - coof; 
        vec3 momentum_coof = (this->velocity * coof) * this->mass;
        vec3 coefficient = (this->velocity * relative_mass) + momentum_coof;
        this->velocity = coefficient / total_mass;

        E = elasticity;
        coof = elasticity * this->mass;
        relative_mass = point.mass - coof;
        momentum_coof = (point.velocity * coof) * point.mass;
        coefficient = (point.velocity * relative_mass) + momentum_coof;
        point.velocity = coefficient / total_mass;*/



        double e = (elasticity + point_elasticity) / 2.0f;
        double e2 = point_elasticity;
        double e1 = elasticity;

        double mass1 = this->mass;
        double mass2 = point.mass;
        double total_mass = mass1 + mass2;

        vec3 v1 = this->velocity;
        vec3 v2 = point.velocity;
        vec3 momentum1 = v1 * mass1;
        vec3 momentum2 = v2 * mass2;
        
        //final velocities using the elastic collision formulas
        /*this->velocity.x = ((mass1 - mass2 * e1) * v1.x + (2 * mass2 * (1 + e1) * v2.x)) / total_mass;
        this->velocity.y = ((mass1 - mass2 * e1) * v1.y + (2 * mass2 * (1 + e1) * v2.y)) / total_mass;
        this->velocity.z = ((mass1 - mass2 * e1) * v1.z + (2 * mass2 * (1 + e1) * v2.z)) / total_mass;

        point.velocity.x = ((mass2 - mass1 * e2) * v2.x + (2 * mass1 * (1 + e2) * v1.x)) / total_mass;      
        point.velocity.y = ((mass2 - mass1 * e2) * v2.y + (2 * mass1 * (1 + e2) * v1.y)) / total_mass;    
        point.velocity.z = ((mass2 - mass1 * e2) * v2.z + (2 * mass1 * (1 + e2) * v1.z)) / total_mass;*/

        
        double impulse = -(1 + e1) * relativeVelocity / total_mass;
        vec3 impulseVector = collisionNormal * impulse;
        this->velocity -= impulseVector * (mass2 / total_mass);

        impulse = -(1 + e2) * relativeVelocity / total_mass;
        impulseVector = collisionNormal * impulse;
        point.velocity += impulseVector * (mass1 / total_mass);

    };

};

vec3 Physics::point_mass::compute_spring_force(const vec3& anchor_pos, const double& length_of_spring_at_rest, const double& spring_constant) {

    //getting the vector spanning from our anchor to our current position => this is the current vector resembeling our spring. NOTE: this vector contains both direction and magnitude
    double current_spring_length = (this->position - anchor_pos).magnitude();
    vec3 direction = (this->position - anchor_pos).normalize();

    //calculating the difference between our current spring length and the length of said spring at rest
    double length_difference = current_spring_length - length_of_spring_at_rest;
  
    //normalizing our current spring vector inorder to get purely its direction and using this formula: F = direction * -spring_constant * length_difference => we get the spring force
    return direction * (-spring_constant * length_difference);

};

vec3 Physics::point_mass::compute_weight_force(const vec3& gravity) {

    return gravity * this->mass;

};

vec3 Physics::point_mass::compute_friction_force(const point_mass& point, const double& friction_coefficient, const vec3& gravity) {

    //friction pushes our point against the direction its trying to go to => friction direction = -direction of velocity
    vec3 friction_direction = this->velocity.normalize() * -1;

    //inorder to get the angle of the slope of which friction occurs between our points we need to find the vector between them and use the formula arctan(vec.z/vec.magnitude);
    vec3 this_to_point = point.position - this->position;
    double horizontal_distance = std::sqrt(this_to_point.x * this_to_point.x + this_to_point.y * this_to_point.y);
    double angle_of_slope = atan2(this_to_point.z, horizontal_distance);
    vec3 normal_force = gravity * this->mass * cos(angle_of_slope);//normal force = weight * cos(angle of slope)

    //using this formula: friction force = friction_direction * friction coefficient * normal force
    vec3 friction_force = friction_direction * normal_force * friction_coefficient;
    vec3 weight_down_slope = (gravity * this->mass).magnitude() * sin(angle_of_slope);//weight_down_slope returns how does the object rotate down on our slope due to gravity
    return weight_down_slope - friction_force;

};

vec3 Physics::point_mass::compute_damping_force(const double& damping_coefficient, const double& max_damping_force) {

    vec3 damping_force = this->velocity * -damping_coefficient;

    if (damping_force.magnitude() > max_damping_force) {

        damping_force = damping_force.normalize() * max_damping_force;

    };

    return damping_force;

};

void Physics::point_mass::add_force(const vec3& force, const double& delta_time) {

    this->net_force += force * delta_time;

};

void Physics::point_mass::apply_forces(const double& delta_time) {

    vec3 acceleration = this->net_force / this->mass;
    this->velocity += acceleration * delta_time;
    this->position += this->velocity * delta_time;

};

void Physics::point_mass::reset_forces() {

    this->net_force = { 0, 0, 0 };//the z-coord value here is 0 and not 1 since net_force vector resembles physics forces and their values, not graphical values or directions.  

};

Physics::point_mass::point_mass(const vec3& position, const vec3& velocity, const double& radius, const double& mass) : 
    
    position(position), velocity(velocity), mass(mass), radius(radius) {

};

void Physics::Body::print_debug_information() {

     for (auto& point : this->points) {

        std::cout << "initial position of vertex " << point.index << " is " << point.position.x << ", " << point.position.y << std::endl;

    };

    int i = 0;
    for (auto& p : this->points) {

        std::cout << "initial position of mass point " << p.index << " is " << p.position.x << ", " << p.position.y << std::endl;
        i++;

    }

    for (int i = 0; i < this->frame.getPointCount(); i++) {

        std::cout << "initial position of soft body point " << i << " is " << this->frame.getPoint(i).x << ", " << this->frame.getPoint(i).y << std::endl;

    }

    std::cout << "point_mass count: " << this->points.size() << std::endl;
    std::cout << "convex shape points count: " << this->frame.getPointCount() << std::endl;
    std::cout << "number of center-to-point vectors: " << this->center_to_point_length.size() << std::endl;
    std::cout << "number of constraints: " << this->constraints.size() << std::endl;

};

void Physics::Body::constrain_points(const point_mass& point_1, const point_mass& point_2) {

    bool constraint_exists = false;

    if (!this->constraints.empty()) {

        for (auto& constraint : this->constraints) {

            if ( (constraint.index_1 == point_1.index && constraint.index_2 == point_2.index) || (constraint.index_1 == point_2.index && constraint.index_2 == point_1.index) ) {

                constraint_exists = true;
                break;

            };

        };

    };

    if (!constraint_exists) {

        this->constraints.emplace_back(point_1.index, point_2.index, (point_1.position - point_2.position).magnitude());

    };

};

void Physics::Body::update_center() {

    this->center = { 0, 0, 1 };
    for (auto const& point : this->points) {

        this->center += points[point.index].position;

    };

    this->center /= (double)this->points.size();

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
        this->frame.setPoint(point.index, sf::Vector2f(point.position.x, point.position.y));
        
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
        this->frame.setPoint(point.index, sf::Vector2f(point.position.x, point.position.y));

    };

    this->set_position(original_pos);
    this->update_center();

};

double Physics::Body::calculate_total_mass() {

    double mass = 0.0f;
    for (const auto& point : this->points) {

        mass += point.mass;

    };

    return mass;

};

Physics::Body::Body(const std::vector<vec3>& positions, const Material& material, const double& mass, const double& radius, const vec3& velocity) : material(material), mass(mass) {

    int total_points = 0;

    double space_manager = radius * 4.0f;// * 2.25 provides best simulation but causes big instabilities in various shapes due to overcrowding, * 3.55 provides the best stability

    std::vector<int> sizes;

    for (int i = 0; i < positions.size(); ++i) {

        int next_index = (i + 1) % positions.size();

        //distance between this position and the next
        double distance = (positions[next_index] - positions[i]).magnitude();

        int n_points = distance / space_manager;

        total_points += n_points;

        sizes.emplace_back(std::move(n_points));

    };

    this->frame.setPointCount(total_points);
    this->points.reserve(total_points);

    double mass_per_point = this->mass / total_points;

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

                point_mass point(this->points[this->points.size() - 1].position, velocity, radius, mass_per_point);

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

                if (positions[next_index].z + radius < point.position.z - space_manager) {

                    point.position.z -= space_manager;

                }
                else if (positions[next_index].z - radius > point.position.z + space_manager) {

                    point.position.z += space_manager;

                };

                if (point.position != positions[next_index]) {
                    
                    this->points.emplace_back(std::move(point));
                    this->points[this->points.size() - 1].index = this->points.size() - 1;
                    this->frame.setPoint(this->points.size() - 1, sf::Vector2f(this->points[this->points.size() - 1].position.x, this->points[this->points.size() - 1].position.y));

                };

            };

        };

    };

    update_center();
    for (auto& point : this->points) {

        int next_index = (point.index + 1) % this->points.size();
        int last_index = (point.index - 1) % this->points.size();

        this->constrain_points(point, this->points[last_index]);
        this->constrain_points(point, this->points[next_index]);

        this->center_to_point_length.emplace_back((point.position - this->center).magnitude());

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

void Physics::Body::collide(const double& delta_time, Body& body, const vec3& gravity) {

    if (this->find_collision(body).depth > 0.0f) {

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

    };

};

void Physics::Body::update(const double& delta_time, const vec3& gravity) {

    update_center();

    double cross_section_area;
    double extension;
    double spring_constant;
    
    vec3 spring_force;
    vec3 damping;

    vec3 weight;
    vec3 acceleration; 

    if (!this->constraints.empty()) {

        for (auto& const constraint : this->constraints) {

            spring_constant = this->material.compute_spring_constant(constraint.length_at_rest);
            spring_force = this->points[constraint.index_1].compute_spring_force(this->points[constraint.index_2].position, constraint.length_at_rest, spring_constant);
            this->points[constraint.index_1].add_force(spring_force, delta_time);
            this->points[constraint.index_2].add_force(spring_force * -1, delta_time);

        };

    };

    for (auto& point : this->points) {

        weight = point.compute_weight_force(gravity);
        point.add_force(weight, delta_time);
        
        spring_constant = this->material.compute_spring_constant(this->center_to_point_length[point.index]);
        spring_force = point.compute_spring_force(this->center, this->center_to_point_length[point.index], spring_constant);        
        point.add_force(spring_force, delta_time);
        //std::cout << "spring_force: " << spring_force.x << ", " << spring_force.y << ", " << spring_force.z << std::endl;
          
        damping = point.compute_damping_force(this->material.compute_damping_coefficient(this->material.damping_ratio, this->mass), spring_force.magnitude());
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

void Physics::UPDATE(const double& delta_time, const vec3& gravity) {

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

            
            this->Bodies.emplace_back(this->mouse_drawn_body_positions, Material::jello(), 400.0f, 2.0f);
            this->Bodies[this->Bodies.size() - 1].frame.setFillColor(sf::Color(0, 250, 0, 250));
            this->Bodies[this->Bodies.size() - 1].index = this->Bodies.size() - 1;

            this->mouse_drawn_body_positions.clear();

        };
        
    };

};

void Physics::TEST(sf::RenderWindow* window, vec3& gravity, bool& start) {

    bool create = false;

    sf::Vector2i mouse = sf::Mouse::getPosition();

    vec3 velocity(0.0f, 0.0f, 0.0f);

    vec3 mouse_pos(mouse.x, mouse.y);

    double r;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

        r = 10.0f;

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
            circle.setPosition(sf::Vector2f(mouse_pos.x, mouse_pos.y));
            std::vector<vec3> vertices;
            for (int i = 0; i < circle.getPointCount(); ++i) {

                vertices.emplace_back(circle.getPoint(i).x + mouse_pos.x, circle.getPoint(i).y + mouse_pos.y, 0);

            };
         
            //creating a body inside the vector
            this->Bodies.emplace_back(vertices, Material::rubber(), 62.0f, 1.0f, velocity);
            this->Bodies[this->Bodies.size() - 1].frame.setFillColor(sf::Color(0, 0, 250, 250));
            this->Bodies[this->Bodies.size() - 1].index = this->Bodies.size() - 1;

            start = false;

            create = false;

        };

    };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {

        std::vector<vec3> rectangleVertices = {

            mouse_pos,

            vec3(mouse_pos.x + 50.0f, mouse_pos.y),

            vec3(mouse_pos.x + 100.0f, mouse_pos.y + 50.0f),

            vec3(mouse_pos.x + 100.0f, mouse_pos.y + 100.0f),

            vec3(mouse_pos.x + 50.0f, mouse_pos.y + 150.0f),

            vec3(mouse_pos.x, mouse_pos.y + 150.0f),

            vec3(mouse_pos.x - 50.0f, mouse_pos.y + 100.0f),

            vec3(mouse_pos.x - 50.0f, mouse_pos.y + 50.0f)

        };

        r = 2.0f;

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

            this->Bodies.emplace_back(rectangleVertices, Material::iron(), 1000.0f, 2.0f, velocity);
            this->Bodies[this->Bodies.size() - 1].frame.setFillColor(sf::Color(1, 50, 90, 250));
            this->Bodies[this->Bodies.size() - 1].index = this->Bodies.size() - 1;

            start = false;

            create = false;

        };

    };

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
