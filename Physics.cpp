#include "physics.h"
#include "Fluids.h"
#include <thread>
#include <iostream>

void Physics::setWindow(sf::RenderWindow* window) {

    this->window = window;

}

void Physics::setFluids(std::shared_ptr<Fluids> fluids) {

    this->fluids = fluids;

}

float Physics::length(const sf::Vector2f& v) {

    return std::sqrt(v.x * v.x + v.y * v.y);

}

sf::Vector2f Physics::multiplyVectors(sf::Vector2f const& A, sf::Vector2f const& B) {

    float x = A.x * B.x;

    float y = A.y * B.y;

    return sf::Vector2f(x, y);

}

float Physics::dot(sf::Vector2f A, sf::Vector2f B) {

    return A.x * B.x + A.y * B.y;

}

float Physics::cross(sf::Vector2f A, sf::Vector2f B) {

    return A.x * B.y - A.y * B.x;

}

sf::Vector2f Physics::rotate(sf::Vector2f const& vector, float  const& angle) {

    float cosAngle = cos(angle);

    float sinAngle = sin(angle);

    sf::Vector2f rotatedVector;

    rotatedVector.x = (vector.x * cosAngle) - (vector.y * sinAngle);

    rotatedVector.y = (vector.x * sinAngle) + (vector.y * cosAngle);

    return rotatedVector;

}

void Physics::create_border(bool is_horizontal, float length, float width, float x, float y) {

    Physics::border Border;

    Border.length = length;

    Border.width = width;

    Border.is_horizontal = is_horizontal;

    Border.center = { x + length / 2, y + width / 2 };

    /*if (!is_horizontal) {

        Border.center = { x + length / 2, y + width / 2 };

    }*/

    sf::RectangleShape rect;

    rect.setSize({ Border.length, Border.width });

    rect.setPosition(x, y);

    rect.setFillColor(sf::Color(255, 0, 0, 50));

    borders.emplace_back(Border);

    rects.emplace_back(rect);

}

void Physics::create_planet(float radius, float x, float y) {

    Physics::planet Planet;

    Planet.radius = radius;

    sf::Vector2f center = { x, y };

    Planet.center = center;

    sf::CircleShape circle(Planet.radius);

    circle.setPosition(Planet.center.x - Planet.radius, Planet.center.y - Planet.radius);

    circle.setFillColor(sf::Color(255, 0, 0, 50));

    planets.emplace_back(Planet);

    circles.emplace_back(circle);

}

void Physics::create_collider(float radius, float x, float y, sf::Vector2f velocity, sf::Color color, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders) {

    Physics::point_mass weighted_point;

    weighted_point.velocity = velocity;

    weighted_point.position = { x, y };

    sf::CircleShape circle(radius);

    circle.setPosition(x - radius, y - radius);

    circle.setFillColor(color);

    colliders.emplace_back(circle);

    points.emplace_back(weighted_point);

}

void Physics::create_colliders_with_constraints(float radius1, float x1, float y1, sf::Vector2f velocity1, float radius2, float x2, float y2, sf::Vector2f velocity2, float distance, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders, std::vector<distance_constraint>& constraints, std::vector<sf::ConvexShape>& Lines) {

    Physics::distance_constraint constraint;

    constraint.distance = distance;

    create_collider(radius1, x1, y1, velocity1, sf::Color::Blue, points, colliders);

    constraint.index0 = points.size() - 1;

    create_collider(radius2, x2, y2, velocity2, sf::Color::Blue, points, colliders);

    constraint.index1 = points.size() - 1;

    constraint.Line.setPointCount(4);

    constraint.Line.setPoint(0, { points[constraint.index0].position.x, points[constraint.index0].position.y - radius1 / 2 });

    constraint.Line.setPoint(1, { points[constraint.index1].position.x, points[constraint.index1].position.y - radius2 / 2 });

    constraint.Line.setPoint(2, { points[constraint.index0].position.x, points[constraint.index0].position.y + radius1 / 2 });

    constraint.Line.setPoint(3, { points[constraint.index1].position.x, points[constraint.index1].position.y + radius1 / 2 });

    constraint.Line.setFillColor(sf::Color::Blue);

    constraints.emplace_back(constraint);

    Lines.emplace_back(constraint.Line);

}

Physics::soft_body Physics::create_soft_body_automatic(Physics& physics, std::vector<sf::Vector2f>& vertices_positions, sf::Vector2f velocity, float radius, sf::Color const& color, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders) {

    Physics::soft_body body;

    sf::Color C(color.r, color.g, color.b, 100);

    body.shape.setFillColor(C);

    int n_shape_points = 0;

    float space_manager = radius * 3.55;// * 2.25 provides best simulation but causes big instabilities in various shapes due to overcrowding colliders/vertices
                                        // * 3.55 provides the best stability

    std::vector<int> sizes;

    for (int i = 0; i < vertices_positions.size(); i++) {

        int next_index = (i + 1) % vertices_positions.size();

        float X = vertices_positions[next_index].x - vertices_positions[i].x;

        float Y = vertices_positions[next_index].y - vertices_positions[i].y;

        sf::Vector2f length(X, Y);

        float distance = physics.length(length);

        int n_points = distance / space_manager;

        n_shape_points += n_points;

        sizes.emplace_back(n_points);

        std::cout << "length: " << distance << std::endl;

        std::cout << "n_points: " << n_points << std::endl;

        std::cout << "sizes: " << sizes[i] << std::endl;

    }

    std::cout << "size of sizes: " << sizes.size() << std::endl;

    std::cout << "total number of points on shape: " << n_shape_points << std::endl;

    body.shape.setPointCount(n_shape_points);

    int index = 0;

    int origin_index = 0;

    for (int i = 0; i < vertices_positions.size(); i++) {
       
        int next_index = (i + 1) % vertices_positions.size();

        std::cout << "Origin point " << i << " at coordinates: " << vertices_positions[i].x << ", " << vertices_positions[i].y << " has an edge with the next point; that consists of " << sizes[i] << " vertices/colliders\n" << std::endl;

        for (int j = 0; j < sizes[i]; j++) {

            Physics::soft_body::vertex vertex;

            if (j == 0) {

                vertex.position = vertices_positions[i];

                physics.create_collider(radius, vertex.position.x, vertex.position.y, velocity, sf::Color::Yellow, points, colliders);

                vertex.index = physics.points.size() - 1;

                body.vertices.emplace_back(vertex);

                body.shape.setPoint(index, vertex.position);

                std::cout << "index here: " << index << " pos: " << vertex.position.x << ", "<< vertex.position.y << std::endl;

                physics.all_soft_bodies_indices.emplace_back(vertex.index);

                index++;

            }

            else {//VERY IMPORTANT NOTE: WE ADD OR SUBTRACT THE RADIUS FROM THE NEXTPOSITION SINCE THE POSITION REFERS TO THE CENTER OF THE COLLIDER AND NOT THE
                   //USUAL ORIGIN POINT OF THE DRAWN CIRCLE. WE ALSO ADD OR SUBTRACT THE SPACEMANAGER FROM THE CURRENT COLLIDER POSITION SINCE ITS WHERE THE NEW
                   //POINT POSITION SHOULD BE IF WE INDEED CREATED THE NEW COLLIDER

                vertex.position = physics.points[physics.points.size() - 1].position;

                if (vertices_positions[next_index].x - radius > vertex.position.x + space_manager) {

                    vertex.position.x += space_manager;

                }

                else if (vertices_positions[next_index].x + radius < vertex.position.x - space_manager) {

                    vertex.position.x -= space_manager;

                }

                if (vertices_positions[next_index].y + radius < vertex.position.y - space_manager) {

                    vertex.position.y -= space_manager;

                }

                else if (vertices_positions[next_index].y - radius > vertex.position.y + space_manager) {

                    vertex.position.y += space_manager;

                }

                if (vertices_positions[next_index] != vertex.position) {

                    physics.create_collider(radius, vertex.position.x, vertex.position.y, velocity, color, points, colliders);

                    vertex.index = physics.points.size() - 1;

                    body.vertices.emplace_back(vertex);

                    body.shape.setPoint(index, vertex.position);

                    physics.all_soft_bodies_indices.emplace_back(vertex.index);

                    index++;

                }

            }//else closing

        }

    }

    //compute the center of mass
    body.center = { 0.f ,0.f };
    for (auto const& v : body.vertices) {

        body.center += physics.points[v.index].position;

    }

    body.center /= (float)body.vertices.size();

    for (auto& v : body.vertices) {

        sf::Vector2f r = physics.points[v.index].position - body.center;

        body.vectors_from_center.emplace_back(r);

    }

    for (auto& v : body.vertices) {

        std::cout << "initial position of vertex " << v.index << " is " << v.position.x << ", " << v.position.y << std::endl;

    }

    int i = 0;
    for (auto& p : physics.points) {

        std::cout << "initial position of mass point " << i << " is " << p.position.x << ", " << p.position.y << std::endl;
        i++;

    }

    for (int i = 0; i < body.shape.getPointCount(); i++) {

        std::cout << "initial position of soft body point " << i << " is " << body.shape.getPoint(i).x << ", " << body.shape.getPoint(i).y << std::endl;

    }

    std::cout << "V size: " << body.vertices.size() << std::endl;
    std::cout << "P size: " << physics.points.size() << std::endl;
    std::cout << "Shape size: " << body.shape.getPointCount() << std::endl;

    physics.soft_bodies_shapes.emplace_back(body.shape);

    physics.soft_bodies.emplace_back(body);

    return body;

}

//Physics::soft_body create_soft_body_automatic(Physics& physics, std::vector<sf::Vector2f>& vertices_positions, sf::Vector2f velocity, float radius, sf::Color const& color) {
//
//    Physics::soft_body body;
//
//    sf::Color C(color.r, color.g, color.b, 100);
//
//    body.shape.setFillColor(C);
//
//    int n_shape_points = 0;
//
//    float space_manager = radius * 2.25;
//
//    std::vector<int> sizes;
//
//    std::vector<std::vector<int>> diagonal_sizes;
//
//    int d_sizes = 0;
//
//    for (int i = 0; i < vertices_positions.size(); i++) {
//
//        int next_index = (i + 1) % vertices_positions.size();
//
//        float X = vertices_positions[next_index].x - vertices_positions[i].x;
//
//        float Y = vertices_positions[next_index].y - vertices_positions[i].y;
//
//        sf::Vector2f length(X, Y);
//
//        float distance = physics.length(length);
//
//        int n_points = distance / space_manager;
//
//        n_shape_points += n_points;
//
//        sizes.emplace_back(n_points);
//
//        std::cout << "length: " << distance << std::endl;
//
//        std::cout << "n_points: " << n_points << std::endl;
//
//        std::cout << "sizes: " << sizes[i] << std::endl;
//
//        /////////////////////////////////////////////
//
//        X = vertices_positions[next_index].x - vertices_positions[i].x;
//
//        Y = vertices_positions[next_index].y - vertices_positions[i].y;
//
//        distance = physics.length(length);
//
//        int base = distance / space_manager;
//
//        ////////////////////////
//
//        X = body.center.x - vertices_positions[next_index].x;
//
//        Y = body.center.y - vertices_positions[next_index].y;
//
//        distance = physics.length(length);
//
//        int from_next_index_to_center = distance / space_manager;
//
//        ////////////////////////
//
//        X = vertices_positions[i].x - body.center.x;
//
//        Y = vertices_positions[i].y - body.center.y;
//
//        distance = physics.length(length);
//
//        int from_center_to_index = distance / space_manager;
//
//        diagonal_sizes.emplace_back(std::vector<int> { base, from_center_to_index, from_next_index_to_center });
//
//        d_sizes += base + from_center_to_index + from_next_index_to_center;
//
//    }
//
//    std::cout << "size of sizes: " << sizes.size() << std::endl;
//
//    std::cout << "total number of points on shape: " << n_shape_points << std::endl;
//
//    body.shape.setPointCount(n_shape_points);
//
//   // body.inner_diagonals_of_shape.resize(d_sizes);
//
//    int index = 0;
//
//    int origin_index = 0;
//
//    for (int i = 0; i < vertices_positions.size(); i++) {
//       
//        int next_index = (i + 1) % vertices_positions.size();
//
//        for (int j = 0; j < sizes[i]; j++) {
//
//            Physics::soft_body::vertex vertex;
//
//            if (j == 0) {
//
//                vertex.position = vertices_positions[i];
//
//                create_collider(physics, radius, vertex.position.x, vertex.position.y, velocity, sf::Color::Yellow);
//
//                vertex.index = physics.points.size() - 1;
//
//                body.vertices.emplace_back(vertex);
//
//                body.shape.setPoint(index, vertex.position);
//
//                std::cout << "index here: " << index << " pos: " << vertex.position.x << ", "<< vertex.position.y << std::endl;
//
//                all_soft_bodies_indices.emplace_back(vertex.index);
//
//                index++;
//
//            }else {//VERY IMPORTANT NOTE: WE ADD OR SUBTRACT THE RADIUS FROM THE NEXTPOSITION SINCE THE POSITION REFERS TO THE CENTER OF THE COLLIDER AND NOT THE
//                   //USUAL ORIGIN POINT OF THE DRAWN CIRCLE. WE ALSO ADD OR SUBTRACT THE SPACEMANAGER FROM THE CURRENT COLLIDER POSITION SINCE ITS WHERE THE NEW
//                   //POINT POSITION SHOULD BE IF WE INDEED CREATED THE NEW COLLIDER
//
//                vertex.position = physics.points[physics.points.size() - 1].position;
//
//                if (vertices_positions[next_index].x - radius > vertex.position.x + space_manager) {
//
//                    vertex.position.x += space_manager;
//
//                }
//
//                else if (vertices_positions[next_index].x + radius < vertex.position.x - space_manager) {
//
//                    vertex.position.x -= space_manager;
//
//                }
//
//                if (vertices_positions[next_index].y + radius < vertex.position.y - space_manager) {
//
//                    vertex.position.y -= space_manager;
//
//                }
//
//                else if (vertices_positions[next_index].y - radius > vertex.position.y + space_manager) {
//
//                    vertex.position.y += space_manager;
//
//                }
//
//                if (vertices_positions[next_index] != vertex.position) {
//
//                    create_collider(physics, radius, vertex.position.x, vertex.position.y, velocity, color);
//
//                    vertex.index = physics.points.size() - 1;
//
//                    body.vertices.emplace_back(vertex);
//
//                    body.shape.setPoint(index, vertex.position);
//
//                    all_soft_bodies_indices.emplace_back(vertex.index);
//
//                    index++;
//
//                }
//
//            }//else closing
//
//        }
//
//    }
//
//    // compute the center of mass
//    body.center = { 0.f ,0.f };
//    for (auto const& v : body.vertices) {
//
//        body.center += physics.points[v.index].position;
//
//    }
//
//    body.center /= (float)body.vertices.size();
//
//    for (auto& v : body.vertices) {
//
//        sf::Vector2f r = physics.points[v.index].position - body.center;
//
//        body.vectors_from_center.emplace_back(r);
//
//    }
//
//    for (auto& v : body.vertices) {
//
//        std::cout << "initial position of vertex " << v.index << " is " << v.position.x << ", " << v.position.y << std::endl;
//
//    }
//
//    int i = 0;
//    for (auto& p : points) {
//
//        std::cout << "initial position of mass point " << i << " is " << p.position.x << ", " << p.position.y << std::endl;
//        i++;
//
//    }
//
//    for (int i = 0; i < body.shape.getPointCount(); i++) {
//
//        std::cout << "initial position of soft body point " << i << " is " << body.shape.getPoint(i).x << ", " << body.shape.getPoint(i).y << std::endl;
//
//    }
//
//    std::cout << "V size: " << body.vertices.size() << std::endl;
//    std::cout << "P size: " << physics.points.size() << std::endl;
//    std::cout << "Shape size: " << body.shape.getPointCount() << std::endl;
//
//    soft_bodies_shapes.emplace_back(body.shape);
//
//    soft_bodies.emplace_back(body);
//
//    ///////////////////////////////
//
//    index = 0;
//    for (int i = 0; i < vertices_positions.size(); i++) {
//
//        int next_index = (i + 1) % vertices_positions.size();
//
//        for (int k = 1; k < 3; k++) {
//
//            for (int j = 0; j < diagonal_sizes[i][k]; j++) {
//
//                Physics::soft_body::vertex vertex;
//
//                sf::Vertex V;
//
//                V.color = C;
//
//                if (j == 0) {
//
//                    vertex.position = vertices_positions[i];
//
//                    create_collider(physics, radius, vertex.position.x, vertex.position.y, velocity, sf::Color::Yellow);
//
//                    vertex.index = physics.points.size() - 1;
//
//                    //body.vertices.emplace_back(vertex);
//
//                    V.position = vertex.position;
//
//                   // body.inner_diagonals_of_shape[index].position = vertex.position;
//
//                    body.inner_diagonals_of_shape.append(V);
//
//                    std::cout << "index here: " << index << " pos: " << vertex.position.x << ", " << vertex.position.y << std::endl;
//
//                    // all_soft_bodies_indices.emplace_back(vertex.index);
//
//                    index++;
//
//                }
//                else {//VERY IMPORTANT NOTE: WE ADD OR SUBTRACT THE RADIUS FROM THE NEXTPOSITION SINCE THE POSITION REFERS TO THE CENTER OF THE COLLIDER AND NOT THE
//                    //USUAL ORIGIN POINT OF THE DRAWN CIRCLE. WE ALSO ADD OR SUBTRACT THE SPACEMANAGER FROM THE CURRENT COLLIDER POSITION SINCE ITS WHERE THE NEW
//                    //POINT POSITION SHOULD BE IF WE INDEED CREATED THE NEW COLLIDER
//
//                    vertex.position = physics.points[physics.points.size() - 1].position;
//
//                    if (body.center.x - radius > vertex.position.x + space_manager) {
//
//                        vertex.position.x += space_manager;
//
//                    }
//
//                    else if (body.center.x + radius < vertex.position.x - space_manager) {
//
//                        vertex.position.x -= space_manager;
//
//                    }
//
//                    if (body.center.y + radius < vertex.position.y - space_manager) {
//
//                        vertex.position.y -= space_manager;
//
//                    }
//
//                    else if (body.center.y - radius > vertex.position.y + space_manager) {
//
//                        vertex.position.y += space_manager;
//
//                    }
//
//                    if (body.center != vertex.position) {
//
//                        create_collider(physics, radius, vertex.position.x, vertex.position.y, velocity, color);
//
//                            vertex.index = physics.points.size() - 1;
//
//                            //body.vertices.emplace_back(vertex);
//
//                            V.position = vertex.position;
//
//                            // body.inner_diagonals_of_shape[index].position = vertex.position;
//
//                            body.inner_diagonals_of_shape.append(V);
//
//                            //all_soft_bodies_indices.emplace_back(vertex.index);
//
//                            index++;
//
//                    }
//
//                }//else closing
//
//            }
//
//        }
//
//    }
//
//    return body;
//
//}

Physics::collision Physics::find_collision(sf::Vector2f const& position) {

    return collision{ sf::Vector2f{0.f, 1.f}, -position.y };

}//collision

Physics::collision Physics::find_collision(sf::Vector2f const& position, planet const& planet) {

    sf::Vector2f delta = position - planet.center;

    float distance = length(delta);

    sf::Vector2f normal = delta / distance;

    float depth = planet.radius - distance;

    return collision{ normal, depth };

}//collision with a planet(static round object)

Physics::collision Physics::find_collision(const point_mass& p1, const point_mass& p2, float radius1, float radius2) {

    sf::Vector2f delta = p1.position - p2.position;

    float EPSILON = 1e-6;

    float distance = length(delta);

    if (distance < EPSILON) {

        return collision{ {0,0}, 0 };

    }

    sf::Vector2f normal = delta / distance;

    float depth = radius1 + radius2 - distance;

    return collision{ normal, depth };

}//wit other points

Physics::collision Physics::find_collision(sf::Vector2f const& position, border const& rect) {

    sf::Vector2f rectPoint = { position.x, rect.center.y };

    if (!rect.is_horizontal) {

        rectPoint = { rect.center.x, position.y };

    }

    sf::Vector2f delta = position - rectPoint;

    float distance = length(delta);

    sf::Vector2f normal = delta / distance;

    float depth = rect.width / 2 - distance;

    if (!rect.is_horizontal) {

        depth = rect.length / 2 - distance;

    }

    return collision{ normal, depth };

}//collision with a border(static rectangle object)

Physics::collision Physics::find_collision(sf::Vector2f const& position, std::vector<planet> const& planets) {

    collision result = { sf::Vector2f(0.f, 0.f), -std::numeric_limits<float>::infinity() };

    for (const auto& planet : planets) {

        sf::Vector2f delta = position - planet.center;

        float distance = length(delta);

        sf::Vector2f normal = delta / distance;

        float depth = planet.radius - distance;

        // Check if the current collision has a larger penetration depth than the current result
        if (depth > result.depth) {

            // Update the result with the current collision
            result = { normal , depth };

        }

    }

    return result;

}//collision with multiple planets(static round objects)

Physics::collision Physics::find_collision(sf::Vector2f const& position, std::vector<border> const& borders) {

    collision result = { sf::Vector2f(0.f, 0.f), -std::numeric_limits<float>::infinity() };

    for (const auto& border : borders) {

        sf::Vector2f rectPoint = { position.x, border.center.y };

        if (!border.is_horizontal) {

            rectPoint = { border.center.x, position.y };

        }

        sf::Vector2f delta = position - rectPoint;

        float distance = length(delta);

        sf::Vector2f normal = delta / distance;

        float depth = border.width / 2 - distance;//

        if (!border.is_horizontal) {

            depth = border.length / 2 - distance;

        }

        // Check if the current collision has a larger penetration depth than the current result
        if (depth > result.depth) {

            // Update the result with the current collision
            result = { normal , depth };

        }

    }

    return result;

}//collision with multiple borders(static rectangle objects)

void Physics::update_gravity_and_positions(float dt, std::vector<point_mass>& points) {

        for (auto& p : points) {

            p.velocity += gravity * dt;

        }

        for (auto& p : points) {

            p.position += p.velocity * dt;

        }

};

void Physics::update(float dt, planet const& myPlanet, std::vector<point_mass>& points) {

    for (auto& p : points) {

        p.velocity += gravity * dt;

    }

    for (auto& p : points) {

        p.position += p.velocity * dt;

    }

    for (auto& p : points) {

        // collision c = find_collision(p.position);
        collision c = find_collision(p.position, myPlanet);

        // check if collision took place
        if (c.depth < 0.f) continue;

        // resolve the constraint
        p.position += c.normal * c.depth;

        // compute the normal & tangential velocity
        auto vn = c.normal * dot(c.normal, p.velocity);
        auto vt = p.velocity - vn;

        // apply bouncing
        vn = multiplyVectors(-elasticity, vn);

        // apply friction
        vt.x *= std::expf(-friction.x * dt);

        vt.y *= std::expf(-friction.y * dt);

        // add up the new velocity
        p.velocity = vn + vt;
    }

}//updating on a planet

void Physics::update(float dt, border const& rect, std::vector<point_mass>& points)
{
    for (auto& p : points) {

        p.velocity += gravity * dt;

    }

    for (auto& p : points) {

        p.position += p.velocity * dt;

    }

    for (auto& p : points) {

        // collision c = find_collision(p.position);
        collision c = find_collision(p.position, rect);

        // check if collision took place
        if (c.depth < 0.f) continue;

        // resolve the constraint
        p.position += c.normal * c.depth;

        // compute the normal & tangential velocity
        auto vn = c.normal * dot(c.normal, p.velocity);
        auto vt = p.velocity - vn;

        // apply bouncing
        vn = multiplyVectors(-elasticity, vn);

        // apply friction
        vt.x *= std::expf(-friction.x * dt);

        vt.y *= std::expf(-friction.y * dt);

        // add up the new velocity
        p.velocity = vn + vt;
    }


}//updating on a border

void Physics::update_planets(float dt, std::vector<planet> const& planets, std::vector<point_mass>& points)
{

    for (auto& p : points) {

        // collision c = find_collision(p.position);
        collision c = find_collision(p.position, planets);

        // check if collision took place
        if (c.depth < 0.f) continue;

        // resolve the constraint
        p.position += c.normal * c.depth;

        // compute the normal & tangential velocity
        auto vn = c.normal * dot(c.normal, p.velocity);
        auto vt = p.velocity - vn;

        // apply bouncing
        vn = multiplyVectors(-elasticity, vn);

        // apply friction
        vt.x *= std::expf(-friction.x * dt);

        vt.y *= std::expf(-friction.y * dt);

        // add up the new velocity
        p.velocity = vn + vt;

    }

}//updating on multiple planets

void Physics::update_borders(float dt, std::vector<border> const& borders, std::vector<point_mass>& points)
{

    for (auto& p : points) {

        // collision c = find_collision(p.position);
        collision c = find_collision(p.position, borders);

        // check if collision took place
        if (c.depth < 0.f) continue;

        // resolve the constraint
        p.position += c.normal * c.depth;

        // compute the normal & tangential velocity
        auto vn = c.normal * dot(c.normal, p.velocity);
        auto vt = p.velocity - vn;

        // apply bouncing
        vn = multiplyVectors(-elasticity, vn);

        // apply friction
        vt.x *= std::expf(friction.x * dt);

        vt.y *= std::expf(-friction.y * dt);

        // add up the new velocity
        p.velocity = vn + vt;

    }

}//updating on multiple borders

void Physics::update_colliders(float dt, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders) {

    for (int i = 0; i < points.size(); i++) {

        for (int j = i + 1; j < points.size(); j++) {

            float radius1 = colliders[i].getRadius();

            float radius2 = colliders[j].getRadius();

            collision c = find_collision(points[i], points[j], radius1, radius2);

            // Check if collision took place
            if (c.depth < 0.f) continue;

            //std::cout << "depth: " << c.depth << std::endl;

            // Resolve the constraint for both colliders
            points[i].position += c.normal * (c.depth / 2);

            points[j].position -= c.normal * (c.depth / 2);

            // Compute relative velocity
            sf::Vector2f relativeVelocity = points[j].velocity - points[i].velocity;

            // Compute the normal component of relative velocity
            float vn =dot(c.normal, relativeVelocity);

            // Compute impulse
            float restitution = elasticity.x;  // Adjust as needed

            float k = -(1 + restitution) * vn / (1 / points[i].mass + 1 /points[j].mass);

             //Apply impulse to update velocities
            points[i].velocity -= (k / points[i].mass) * c.normal;

            points[j].velocity += (k / points[j].mass) * c.normal;

            // Apply friction
            float frictionCoefficient = 0.2f;  // Adjust as needed

            auto tangent = relativeVelocity - vn * c.normal;

            auto vt = length(tangent);

            if (vt > 0.0f) {

                auto friction = std::min(frictionCoefficient * std::fabs(k), vt) * (tangent / vt);  // Corrected line

                points[i].velocity -= (friction / points[i].mass);

                points[j].velocity += (friction / points[j].mass);

            }

        }

    }

}

void Physics::update_constraints(float dt, std::vector<point_mass>& points, std::vector<distance_constraint>& constraints) {

    for (auto& c : constraints) {

        auto p0 = points[c.index0].position;

        auto p1 = points[c.index1].position;

        auto& v0 = points[c.index0].velocity;

        auto& v1 = points[c.index1].velocity;

        auto delta = p1 - p0;

        auto distance = length(delta);

        auto direction = delta / distance;

        auto required_delta = direction * c.distance;

        auto force = spring_force * (required_delta - delta);

        v0 -= force * dt;

        v1 += force * dt;

        auto vrel = dot(v1 - v0, direction);

        auto damping_factor = exp(-spring_damping * dt);

        auto new_vrel = vrel * damping_factor;

        auto vrel_delta = new_vrel - vrel;

        v0.x -= vrel_delta / 2.0;

        v0.y -= vrel_delta / 2.0;

        v1.x += vrel_delta / 2.0;

        v1.y += vrel_delta / 2.0;

    }

}//updating on constraints

void Physics::update_soft_bodies(float dt, std::vector<point_mass>& points) {

    for (auto& b : soft_bodies)
    {
        // compute the center of mass
        b.center = { 0, 0 };

        for (auto const& v : b.vertices) {

            b.center += points[v.index].position;

        }

        b.center /= (float)b.vertices.size();

        // compute the shape rotation angle
        float A = 0.f, B = 0.f;

        int i = 0;
        for (auto& v : b.vertices) {

            sf::Vector2f r = v.position - b.center;

            A += dot(b.vectors_from_center[i], r);

            B += cross(b.vectors_from_center[i], r);

            //std::cout << "A: " << A << " | " << "B: " << B << std::endl;

            i++;

        }

        float angle = -atan2(B, A);

        // apply spring forces
        i = 0;
        for (auto& v : b.vertices) {

            auto damping_factor = exp(-spring_damping * dt);

            sf::Vector2f target = b.center + rotate(b.vectors_from_center[i], angle);

            sf::Vector2f delta = target - points[v.index].position;

            points[v.index].velocity *= damping_factor;

            points[v.index].velocity += spring_force * delta * dt;

            i++;

        }

    }

}

void Physics::UPDATE(float dt, std::vector<planet> const& planets, std::vector<border> const& borders, std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders, std::vector<distance_constraint>& constraints) {

        const int numSubsteps = 1; // Adjust the number of substeps

        float substepDt = dt / numSubsteps;

        for (int i = 0; i < numSubsteps; i++) {

            update_gravity_and_positions(substepDt, points);

            update_colliders(substepDt, points, colliders);

            update_constraints(substepDt, points, constraints);

            update_soft_bodies(substepDt, points);

            update_borders(substepDt, borders, points);

            update_planets(substepDt, planets, points);

        }

}

void Physics::render_soft_bodies(std::vector<point_mass>& points) {


    for (int i = 0; i < soft_bodies.size(); i++) {

        window->draw(soft_bodies_shapes[i]);

        for (int j = 0; j < soft_bodies[i].vertices.size(); j++) {

            soft_bodies_shapes[i].setPoint(j, { points[soft_bodies[i].vertices[j].index].position.x, points[soft_bodies[i].vertices[j].index].position.y });

        }

    }

}

void Physics::render_colliders(std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders) {

    for (int i = 0; i < colliders.size(); i++) {

        colliders[i].setPosition(points[i].position);

        window->draw(colliders[i]);

    }

    /*for (int i = 0; i < colliders.size(); i++) {

        if (all_soft_bodies_indices.empty()) {

            window->draw(colliders[i]);

        }

        else {

            int index = i % all_soft_bodies_indices.size();

            if (all_soft_bodies_indices[index] != i) {

                window->draw(colliders[i]);

            }

        }

    }*/

}

void Physics::render_constraints(std::vector<point_mass>& points, std::vector<distance_constraint>& constraints, std::vector<sf::ConvexShape>& Lines) {

    int i = 0;

    for (auto& line : Lines) {

        window->draw(line);

        line.setPoint(0, { points[constraints[i].index0].position.x, points[constraints[i].index0].position.y - 1 });

        line.setPoint(1, { points[constraints[i].index1].position.x, points[constraints[i].index1].position.y - 1 });

        line.setPoint(2, { points[constraints[i].index0].position.x, points[constraints[i].index0].position.y + 1 });

        line.setPoint(3, { points[constraints[i].index1].position.x, points[constraints[i].index1].position.y + 1 });

        i++;

    }

}

//void Physics::render_lines() {
//
//    for (auto& c : constraints) {
//
//        for (int j = 0; j < c.line.getVertexCount(); j++) {
//
//            c.line[j].position = { points[c.indices[j]].position.x, points[c.indices[j]].position.y };
//
//        }
//
//        window->draw(c.line);
//
//    }
//
//}

void Physics::render_circles() {


    for (const auto& planet : circles) {

        window->draw(planet);

    }

}

void Physics::render_rects() {

    for (const auto& border : rects) {

        window->draw(border);

    }

}

void Physics::RENDER(std::vector<point_mass>& points, std::vector<sf::CircleShape>& colliders, std::vector<distance_constraint>& constraints, std::vector<sf::ConvexShape>& Lines) {

    render_circles();

    render_rects();

    render_colliders(points, colliders);

    render_soft_bodies(points);

    render_constraints(points, constraints, Lines);

    //render_lines();

   /* if (!blood_colliders.empty() && !blood_points.empty()) {

        render_colliders(blood_points, blood_colliders);

    }*/

}

void Physics::collider_control(std::vector<point_mass>& Points, std::vector<sf::CircleShape>& Colliders) {

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        if (!Held.load()) {

            Held = true;

        }

    }

    else {

        if (Held.load()) {

            Held.store(false);

            index = -1;

        }

    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

    sf::Vector2i prev_mousePos;
    
    if (Held.load() && !points.empty()) {

        wasHeld.store(true);

        if (mousePos.x >= 0 && mousePos.x <= 1920 && mousePos.y >= 0 && mousePos.y <= 1080) {

            for (int i = 0; i < points.size(); i++) {

                if (mousePos.x >= points[i].position.x - colliders[i].getRadius() &&
                    mousePos.x <= points[i].position.x + colliders[i].getRadius() &&
                    mousePos.y >= points[i].position.y - colliders[i].getRadius() &&
                    mousePos.y <= points[i].position.y + colliders[i].getRadius()) {

                    if (index == -1) {

                        index.store(i);

                        Valid = true;

                    }

                }

                if (Valid.load() && index == i) {

                    float x = mousePos.x;

                    float y = mousePos.y;

                    colliders[i].setPosition({ x - colliders[i].getRadius(), y - colliders[i].getRadius() });

                    points[i].position.x = x;

                    points[i].position.y = y;

                    points[i].velocity = { 0, 0 };

                    if (!index_caught.load()) {

                        point_index.store(i);

                        index_caught.store(true);

                    }

                    prev_mousePos = mousePos;

                }

            }

        }

    }

    if (!Held.load() && wasHeld.load()) {

        Valid = false;

        wasHeld.store(false);

        index_caught.store(false);

    }

}

void Physics::collider_control_thread(std::vector<point_mass>& Points, std::vector<sf::CircleShape>& Colliders)
{

    while (true) {

        collider_control(Points, Colliders);

    }

}

void Physics::start_collider_control_thread(std::vector<point_mass>& Points, std::vector<sf::CircleShape>& Colliders)
{

    std::thread collider_movement([this, &Points, &Colliders]() {

        this->collider_control_thread(Points, Colliders);

        });

    collider_movement.detach();

}

void Physics::TEST(sf::RenderWindow* window, Physics& physics, bool& start) {

    bool create = false;

    //colliders
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

        sf::Vector2i mouse_pos = sf::Mouse::getPosition();

        sf::Vector2f velocity = sf::Vector2f(0, 0);

        float r = 10;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {

            velocity.x = 3000;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {

            velocity.x = -3000;

            create = true;

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

            velocity.y = -3000;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

            velocity.y = 3000;

            create = true;

        }

        if (start && create) {

            create_collider(r, mouse_pos.x, mouse_pos.y, velocity, sf::Color::Blue, points, colliders);

            start = false;

            create = false;

        }

    }

    //constraints
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {

        sf::Vector2i mouse_pos = sf::Mouse::getPosition();

        sf::Vector2f velocity = sf::Vector2f(0, 0);

        float r = 10;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {

            velocity.x = 900;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {

            velocity.x = -900;

            create = true;

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

            velocity.y = -900;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

            velocity.y = 900;

            create = true;

        }

        if (start && create) {

            create_colliders_with_constraints(r, mouse_pos.x, mouse_pos.y, velocity, r, mouse_pos.x + 100, mouse_pos.y, velocity, 100, points, colliders, constraints, Lines);

            start = false;

            create = false;

        }

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {

        sf::Vector2i mouse_pos = sf::Mouse::getPosition();

        sf::Vector2f velocity = sf::Vector2f(0, 0);

        std::vector<sf::Vector2f> positions;

        std::vector<sf::Vector2f> rectangleVertices = {
         
         /////////////////////////////////////////////////////////////CREATES A SQUARE
         
            //sf::Vector2f(mouse_pos.x, mouse_pos.y),//left

            //sf::Vector2f(mouse_pos.x + 200, mouse_pos.y),//right

            //sf::Vector2f(mouse_pos.x + 200, mouse_pos.y + 200),//bottom right

            //sf::Vector2f(mouse_pos.x, mouse_pos.y + 200)//bottom left


         /////////////////////////////////////////////////////////////CREATES A TRAPEZOID

                            /*  sf::Vector2f(mouse_pos.x, mouse_pos.y),

                              sf::Vector2f(mouse_pos.x + 100, mouse_pos.y),

                              sf::Vector2f(mouse_pos.x + 200, mouse_pos.y + 100),

                              sf::Vector2f(mouse_pos.x - 100, mouse_pos.y + 100),*/

         /////////////////////////////////////////////////////////////CREATES AN OCTAGON

                             sf::Vector2f(mouse_pos.x, mouse_pos.y),         

                             sf::Vector2f(mouse_pos.x + 50, mouse_pos.y),

                             sf::Vector2f(mouse_pos.x + 100, mouse_pos.y + 50),

                             sf::Vector2f(mouse_pos.x + 100, mouse_pos.y + 100),

                             sf::Vector2f(mouse_pos.x + 50, mouse_pos.y + 150),

                             sf::Vector2f(mouse_pos.x, mouse_pos.y + 150),

                             sf::Vector2f(mouse_pos.x - 50, mouse_pos.y + 100),

                             sf::Vector2f(mouse_pos.x - 50, mouse_pos.y + 50),
                                          
        };

        float r = 4;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {

            velocity.x = 900;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {

            velocity.x = -900;

            create = true;

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

            velocity.y = -900;

            create = true;

        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

            velocity.y = 900;

            create = true;

        }

        if (start && create) {

            create_soft_body_automatic(physics, rectangleVertices, velocity, r, sf::Color::Blue, points, colliders);

            start = false;

            create = false;

        }

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {

        gravity.y++;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {

        gravity.x++;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {

        gravity.x--;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {

        gravity.y--;

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {

        gravity.x = 0;

        gravity.y = 0;

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {

        start = true;

    }

}
