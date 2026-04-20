#pragma once
#include <SFML/Graphics.hpp>

class GroundObstacle {
private:
    sf::RectangleShape body;
    sf::Vector2f position;
    float speed;
    float width;
    float height;

public:
    GroundObstacle(float startX);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOffScreen() const;
    void reset(float startX);
    void setSpeed(float newSpeed);
};