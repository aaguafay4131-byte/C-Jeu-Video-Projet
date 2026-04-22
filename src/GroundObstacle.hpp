#pragma once
#include <SFML/Graphics.hpp>

class GroundObstacle {
private:
    sf::Texture spriteSheet;
    sf::Sprite sprite;
    sf::Vector2f position;
    float speed;
    float width;
    float height;
    int cactusType; // 0 = small, 1 = large

public:
    GroundObstacle(float startX);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOffScreen() const;
    void reset(float startX);
    void setSpeed(float newSpeed);
};