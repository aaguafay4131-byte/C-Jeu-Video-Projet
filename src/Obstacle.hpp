#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
public:
    Obstacle(float x, float y, float w, float h, float spd, bool isAerial = false);

    void update();
    void draw(sf::RenderWindow& window) const;
    bool isOffScreen() const;

    sf::FloatRect getGlobalBounds() const;

private:
    sf::RectangleShape shape;
    float speed;
};