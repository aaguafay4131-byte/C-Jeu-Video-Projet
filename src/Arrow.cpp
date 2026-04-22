#include "Arrow.hpp"
#include <cmath>
#include <cstdlib>

Arrow::Arrow(float startX) {
    width = 40.0f;
    height = 12.0f;
    speed = 300.0f;
    
    // Arrow shaft (main body)
    shaft.setSize(sf::Vector2f(width, 6));
    shaft.setFillColor(sf::Color(139, 69, 19)); // Brown
    
    // Arrow tip (triangle)
    tip.setRadius(8);
    tip.setPointCount(3);
    tip.setFillColor(sf::Color(192, 192, 192)); // Silver/gray
    
    // Fletching (feathers at the back)
    fletching1.setSize(sf::Vector2f(8, 8));
    fletching1.setFillColor(sf::Color(255, 69, 0));
    fletching1.setRotation(45);
    
    fletching2.setSize(sf::Vector2f(8, 8));
    fletching2.setFillColor(sf::Color(255, 69, 0));
    fletching2.setRotation(-45);
    
    // Randomize arrow height
    int heightType = rand() % 4;
    float arrowY;
    
    switch(heightType) {
        case 0: arrowY = 430.0f; break;
        case 1: arrowY = 445.0f; break;
        case 2: arrowY = 460.0f; break;
        case 3: arrowY = 472.0f; break;
        default: arrowY = 445.0f;
    }
    
    position = sf::Vector2f(startX, arrowY);
    updatePartsPosition();
}

void Arrow::update(float deltaTime) {
    position.x -= speed * deltaTime;
    updatePartsPosition();
}

void Arrow::updatePartsPosition() {
    shaft.setPosition(position.x, position.y);
    tip.setPosition(position.x + width - 8, position.y - 4);
    fletching1.setPosition(position.x - 2, position.y - 4);
    fletching2.setPosition(position.x - 2, position.y + 2);
}

void Arrow::draw(sf::RenderWindow& window) {
    window.draw(shaft);
    window.draw(tip);
    window.draw(fletching1);
    window.draw(fletching2);
}

sf::FloatRect Arrow::getBounds() const {
    sf::FloatRect bounds;
    bounds.left = position.x;
    bounds.top = position.y - 6;
    bounds.width = width + 10;
    bounds.height = height + 8;
    return bounds;
}

bool Arrow::isOffScreen() const {
    return position.x + width < 0;
}

void Arrow::reset(float startX) {
    position.x = startX;
    updatePartsPosition();
}

void Arrow::setSpeed(float newSpeed) {
    speed = newSpeed;
}