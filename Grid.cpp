#pragma 
#include"Grid.h"
#include<SFML/Graphics.hpp>;
#include<vector>
#include<stdexcept>
#include<sstream>
#include<iomanip>

using namespace sf;

const int FREE = 0;

const int GRID_WIDTH = 1920;

const int GRID_HEIGHT = 1080;

//-------------------------------------------------------------Field
Grid::Field::Field() : value(0), ScreenGrid(nullptr) {}

Grid::Field::Field(int value) {

    this->value = value;

}


int Grid::Field::getValue() {

    return this->value;

}


void Grid::Field::setValue(int value) {

        this->value = value;
    
}

//--------------------------------------------------------------------------------------------------------------------AntField
std::vector<std::vector<Grid::Field>> Grid::createFields(std::vector<std::vector<int>> fields) {

    if (fields.empty()) {

        throw std::invalid_argument("Fields array must not be empty");
    }

    std::vector<std::vector<Field>> ret(fields.size(), std::vector<Field>());

    for (int i = 0; i < fields.size(); i++) {

        ret[i] = std::vector<Field>(fields[i].size());

        for (int j = 0; j < fields[i].size(); j++) { 

                ret[i][j] = std::move(fields[i][j]);

        }
    }

    return ret;
}

Grid::Grid(std::vector<std::vector<int>> fields) {//class constructor

    this->constructor_fields = fields;

    this->fields = createFields(fields);

    this->is_red.resize(fields.size(), std::vector<bool>(fields[0].size(), false));

    this->is_drawn.resize(fields.size(), std::vector<bool>(fields[0].size(), false));

    this->cell_length.resize(fields.size(), std::vector<int>(fields[0].size()));

    this->density.resize(fields.size(), std::vector<float>(fields[0].size()));

    this->velocity.resize(fields.size(), std::vector<sf::Vector2f>(fields[0].size()));

    this->new_velocity.resize(fields.size(), std::vector<sf::Vector2f>(fields[0].size()));

    this->m.resize(fields.size(), std::vector<sf::Vector2f>(fields[0].size(), sf::Vector2f(1,1)));

    this->viscosity.resize(fields.size(), std::vector<float>(fields[0].size()));

    this->pressure.resize(fields.size(), std::vector<float>(fields[0].size()));

    this->scalar.resize(fields.size(), std::vector<float>(fields[0].size(), 1));

    }

bool Grid::getRed(int y, int x) {

    return this->is_red[y][x];

}

void Grid::setRed(int y, int x) {

    this->is_red[y][x] = true;

}

bool Grid::getDrawn(int y, int x) {

    return this->is_drawn[y][x];

}

void Grid::setDrawn(int y, int x) {

    this->is_drawn[y][x] = true;

}

Grid::Field& Grid::getField(int x, int y) {//getField

    if (x >= this->fields.size() || y >= this->fields[x].size()) {

        throw std::out_of_range("coordinates are bigger than vector");

    }

    else {

        return this->fields[x][y];
    }

}

std::vector<std::vector<int>> Grid::getConstructorFields() {//responsible for returning the value of the constructor parameter
    //through the mimic, thus pointing at the AntField used in 
   //the Ant's construction

    return this->constructor_fields;

}

int Grid::getConstructorFields(int y, int x) {//responsible for returning the value of the constructor parameter
    //through the mimic, thus pointing at the AntField used in 
   //the Ant's construction

    return constructor_fields[y][x];

}

int Grid::getNumberOfRows() {//returns the number of rows that our array is representing
    //through the length of the outer container

    return constructor_fields.size();

}

int Grid::getNumberOfColumns() {//returns the number of columns that our array is representing
    //through the length of the inner container

    return constructor_fields[0].size();

}

int  Grid::getNumberOfColumns(int y) {//returns the number of columns that our array is representing
    //through the length of the inner container but with a parameter
   //in order to be used in loops correctly if need be

    return constructor_fields[y].size();

}

int Grid::getBloodLength(int y, int x) {

    return this->cell_length[y][x];

}

void Grid::setBloodLength(int y, int x, int z) {

    this->cell_length[y][x] = z;

}

float Grid::getDensity(int y, int x) {

    return this->density[y][x];

}

void Grid::setDensity(int y, int x, int z) {

    this->density[y][x] = z;

}

sf::Vector2f Grid::getVelocity(int y, int x) {

    return this->velocity[y][x];

}

void Grid::setVelocity(int y, int x, float v, float u) {

    this->velocity[y][x].x = u;

    this->velocity[y][x].y = v;

}

sf::Vector2f Grid::getNewVelocity(int y, int x) {

    return this->new_velocity[y][x];

}

void Grid::setNewVelocity(int y, int x, float v, float u) {

    this->new_velocity[y][x].x = u;

    this->new_velocity[y][x].y = v;

}

sf::Vector2f Grid::getM(int y, int x) {

    return this->m[y][x];

}

void Grid::setM(int y, int x, float v, float u) {

    this->m[y][x].x = u;

    this->m[y][x].y = v;

}

float Grid::getViscosity(int y, int x) {

    return this->viscosity[y][x];

}

void Grid::setViscosity(int y, int x, int z) {

    this->viscosity[y][x] = z;

}

float Grid::getPressure(int y, int x) {

    return this->pressure[y][x];

}

void Grid::setPressure(int y, int x, int z) {

    this->pressure[y][x] = z;

}

float Grid::getScalar(int y, int x) {

    return this->scalar[y][x];

}

void Grid::setScalar(int y, int x, int z) {

    this->scalar[y][x] = z;

}


