#include "Obstacle.hpp"

Obstacle::Obstacle(float x, float y, float w, float h, float spd, bool isAerial) 
    : speed(spd) {
    shape.setSize({w, h});
    shape.setFillColor(isAerial ? sf::Color(255, 140, 0) : sf::Color(34, 139, 34));
    shape.setPosition(x, y);
}

void Obstacle::update() {
    shape.move(-speed, 0);
}

void Obstacle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Obstacle::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

bool Obstacle::isOffScreen() const {
    return shape.getPosition().x + shape.getSize().x < 0;
}