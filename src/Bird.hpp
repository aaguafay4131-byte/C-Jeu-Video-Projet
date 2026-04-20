#pragma once
#include <SFML/Graphics.hpp>

class Bird {
private:
    sf::RectangleShape body;
    sf::RectangleShape leftWing;
    sf::RectangleShape rightWing;
    sf::Vector2f position;
    float speed;
    float width;
    float height;
    float wingAngle;
    float wingAnimationTime;
    bool isFlying;

public:
    Bird(float startX);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOffScreen() const;
    void reset(float startX);
    void setSpeed(float newSpeed);
};