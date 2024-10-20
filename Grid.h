#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Grid
{

public:

    struct Field {

    public:

        int value;

        bool is_red = false;

        bool is_drawn = false;

        sf::Vector2u pos;

        Field(int value, const sf::Vector2u& pos);

    };//class Field bracket  

    std::vector<std::vector<Field>> fields;

    Grid(std::vector<std::vector<int>>& fields);

    Field getField(int y, int x);

    sf::Vector2u get_size();

    int getValue(int y, int x);

    void setValue(int y, int x, const int value);

    bool getRed(int y, int x);

    void setRed(int y, int x);

    bool getDrawn(int y, int x);

    void setDrawn(int y, int x);

};//class Grid bracket

