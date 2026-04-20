#include "GroundObstacle.hpp"
#include <cstdlib>

GroundObstacle::GroundObstacle(float startX) {
    // Randomize cactus size
    width = 15 + (rand() % 20);
    height = 30 + (rand() % 25);
    speed = 300.0f;
    
    body.setSize(sf::Vector2f(width, height));
    body.setFillColor(sf::Color(34, 139, 34)); // Forest green
    
    position = sf::Vector2f(startX, 500 - height);
    body.setPosition(position);
}

void GroundObstacle::update(float deltaTime) {
    position.x -= speed * deltaTime;
    body.setPosition(position);
}

void GroundObstacle::draw(sf::RenderWindow& window) {
    window.draw(body);
}

sf::FloatRect GroundObstacle::getBounds() const {
    return body.getGlobalBounds();
}

bool GroundObstacle::isOffScreen() const {
    return position.x + width < 0;
}

void GroundObstacle::reset(float startX) {
    position.x = startX;
    body.setPosition(position);
}

void GroundObstacle::setSpeed(float newSpeed) {
    speed = newSpeed;
}