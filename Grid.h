#pragma 
#include<SFML/Graphics.hpp>
#include<vector>
#include<stdexcept>
#include<sstream>
#include<iomanip>
#include<memory>

using namespace sf;

class Grid : public std::enable_shared_from_this<Grid>
{

public:

    class Field {

        int value;

        Grid* ScreenGrid;

     public:

        Field();

        Field(int value);

        int getValue();

        void setValue(int value);

    };//class Field bracket  

public:

    static const int FREE = 0;

    const int GRID_WIDTH = 1920;

    const int GRID_HEIGHT = 1080;

    std::vector<std::vector<Field>> fields;

    std::vector<std::vector<int>> constructor_fields;

    std::vector<std::vector<bool>> is_red;

    std::vector<std::vector<bool>> is_drawn;

    std::vector<std::vector<int>> cell_length;

    std::vector<std::vector<float>> density;

    std::vector<std::vector<sf::Vector2f>> velocity;

    std::vector<std::vector<sf::Vector2f>> new_velocity;

    std::vector<std::vector<float>> viscosity;

    std::vector<std::vector<float>> pressure;

    std::vector<std::vector<float>> scalar;

    std::vector<std::vector<sf::Vector2f>> m;

    std::vector<std::vector<Field>> createFields(std::vector<std::vector<int>> fields);

    Grid(std::vector<std::vector<int>> fields);

    Field& getField(int y, int x);

    std::vector<std::vector<int>> getConstructorFields();

    int getConstructorFields(int y, int x);

    int getNumberOfRows();

    int getNumberOfColumns();

    int getNumberOfColumns(int y);

    bool getRed(int y, int x);

    void setRed(int y, int x);

    bool getDrawn(int y, int x);

    void setDrawn(int y, int x);

    int getBloodLength(int y, int x);

    void setBloodLength(int y, int x, int z);

    float getDensity(int y, int x);

    void setDensity(int y, int x, int z);

    sf::Vector2f getVelocity(int y, int x);

    void setVelocity(int y, int x, float v, float u);

    sf::Vector2f getNewVelocity(int y, int x);

    void setNewVelocity(int y, int x, float v, float u);

    sf::Vector2f getM(int y, int x);

    void setM(int y, int x, float v, float u);

    float getViscosity(int y, int x);

    void setViscosity(int y, int x, int z);

    float getPressure(int y, int x);

    void setPressure(int y, int x, int z);

    float getScalar(int y, int x);

    void setScalar(int y, int x, int z);

};//class Grid bracket

