#include"Grid.h"

//-------------------------------------------------------------Field

Grid::Field::Field(int value, const sf::Vector2u& pos) {

    this->value = value;
    this->pos = pos;

};


int Grid::getValue(int y, int x) {

    return this->fields[y][x].value;

};


void Grid::setValue(int y, int x, const int value) {

    this->fields[y][x].value = value;

};

bool Grid::getRed(int y, int x) {

    return this->fields[y][x].is_red;

};

void Grid::setRed(int y, int x) {

    this->fields[y][x].is_red = true;

};

bool Grid::getDrawn(int y, int x) {

    return this->fields[y][x].is_drawn;

};

void Grid::setDrawn(int y, int x) {

    this->fields[y][x].is_drawn = true;

};

Grid::Grid(std::vector<std::vector<int>>& fields) {//class constructor
    this->fields.resize(fields.size());
    for (int i = 0; i < fields.size(); i++) {

        for (int j = 0; j < fields[i].size(); j++) {

            this->fields[i].emplace_back(Field(fields[i][j], sf::Vector2u(j, i)));////////////

        };

    };

};

Grid::Field Grid::getField(int y, int x) {//getField

    if (y >= this->fields.size() || x >= this->fields[y].size()) {

        std::cout << "coordinates are bigger than vector" << std::endl;;

    }

    else {

        return this->fields[y][x];

    };

};

sf::Vector2u Grid::get_size() {

    return sf::Vector2u(this->fields.size(), this->fields[0].size());

};



