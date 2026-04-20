#include "Bird.hpp"
#include <cmath>
#include <cstdlib>

Bird::Bird(float startX) {
    width = 30.0f;
    height = 20.0f;
    speed = 300.0f;
    
    body.setSize(sf::Vector2f(width, height));
    
    // Different bird heights to require different dodging strategies
    // Standing dino height: 30px (from Y=470 to Y=440)
    // Crouching dino height: 18px (from Y=482 to Y=464)
    int heightType = rand() % 4;
    float birdY;
    std::string birdType;
    
    switch(heightType) {
        case 0:
            birdY = 430.0f;  // High bird - must jump
            birdType = "high";
            body.setFillColor(sf::Color(255, 69, 0)); // Orange-red
            break;
        case 1:
            birdY = 445.0f;  // Mid-high bird - must jump
            birdType = "mid-high";
            body.setFillColor(sf::Color(255, 100, 0)); // Dark orange
            break;
        case 2:
            birdY = 460.0f;  // Mid-low bird - can jump or crouch
            birdType = "mid-low";
            body.setFillColor(sf::Color(255, 140, 0)); // Orange
            break;
        case 3:
            birdY = 472.0f;  // Low bird - must crouch (hits standing dino)
            birdType = "low";
            body.setFillColor(sf::Color(255, 180, 0)); // Light orange
            break;
        default:
            birdY = 445.0f;
            body.setFillColor(sf::Color(255, 69, 0));
    }
    
    position = sf::Vector2f(startX, birdY);
    body.setPosition(position);
    
    // Setup wings
    leftWing.setSize(sf::Vector2f(width * 0.7f, height * 0.3f));
    rightWing.setSize(sf::Vector2f(width * 0.7f, height * 0.3f));
    leftWing.setFillColor(body.getFillColor());
    rightWing.setFillColor(body.getFillColor());
    
    wingAngle = 0;
    wingAnimationTime = 0;
    isFlying = true;
}

void Bird::update(float deltaTime) {
    // Move bird from right to left
    position.x -= speed * deltaTime;
    body.setPosition(position);
    
    // Animate wings
    wingAnimationTime += deltaTime * 10.0f;
    wingAngle = sin(wingAnimationTime) * 45.0f;
    
    // Update wing positions
    leftWing.setPosition(position.x - width * 0.2f, position.y + height * 0.2f);
    leftWing.setRotation(wingAngle);
    
    rightWing.setPosition(position.x + width * 0.5f, position.y + height * 0.2f);
    rightWing.setRotation(-wingAngle);
}

void Bird::draw(sf::RenderWindow& window) {
    // Draw wings
    window.draw(leftWing);
    window.draw(rightWing);
    
    // Draw body
    window.draw(body);
    
    // Draw eye
    sf::CircleShape eye(3);
    eye.setFillColor(sf::Color::White);
    eye.setPosition(position.x + width - 8, position.y + 5);
    window.draw(eye);
    
    sf::CircleShape pupil(1.5f);
    pupil.setFillColor(sf::Color::Black);
    pupil.setPosition(position.x + width - 7, position.y + 6);
    window.draw(pupil);
}

sf::FloatRect Bird::getBounds() const {
    return body.getGlobalBounds();
}

bool Bird::isOffScreen() const {
    return position.x + width < 0;
}

void Bird::reset(float startX) {
    position.x = startX;
    body.setPosition(position);
}

void Bird::setSpeed(float newSpeed) {
    speed = newSpeed;
}