#pragma once
#include <SFML/Graphics.hpp>

class Arrow {
private:
    sf::RectangleShape shaft;
    sf::CircleShape tip;
    sf::RectangleShape fletching1;
    sf::RectangleShape fletching2;
    sf::Vector2f position;
    float speed;
    float width;
    float height;
    
    void updatePartsPosition();

public:
    Arrow(float startX);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOffScreen() const;
    void reset(float startX);
    void setSpeed(float newSpeed);
};