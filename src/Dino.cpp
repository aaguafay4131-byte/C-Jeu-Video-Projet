#include "Dino.hpp"
#include <iostream>

Dino::Dino() {
    width = 30.0f;
    standingHeight = 30.0f;
    crouchHeight = 18.0f;
    
    body.setSize(sf::Vector2f(width, standingHeight));
    body.setFillColor(sf::Color::Black);
    position = sf::Vector2f(100, GROUND_Y - standingHeight);
    body.setPosition(position);
    velocityY = 0;
    isGrounded = true;
    isJumping = false;
    isCrouching = false;
}

void Dino::update(float deltaTime) {
    // Don't apply gravity or movement if crouching on ground
    if (!isCrouching) {
        // Apply gravity
        velocityY += GRAVITY * deltaTime;
        position.y += velocityY * deltaTime;
        
        // Ground collision
        if (position.y >= GROUND_Y - (isCrouching ? crouchHeight : standingHeight)) {
            position.y = GROUND_Y - (isCrouching ? crouchHeight : standingHeight);
            velocityY = 0;
            isGrounded = true;
            isJumping = false;
        }
    }
    
    body.setPosition(position);
}

void Dino::jump() {
    // Can't jump while crouching
    if (isGrounded && !isCrouching) {
        velocityY = JUMP_FORCE;
        isGrounded = false;
        isJumping = true;
    }
}

void Dino::crouch() {
    if (isGrounded && !isJumping) {
        isCrouching = true;
        body.setSize(sf::Vector2f(width, crouchHeight));
        // Adjust position to keep feet on ground
        position.y = GROUND_Y - crouchHeight;
        body.setPosition(position);
    }
}

void Dino::stand() {
    if (isCrouching) {
        isCrouching = false;
        body.setSize(sf::Vector2f(width, standingHeight));
        position.y = GROUND_Y - standingHeight;
        body.setPosition(position);
    }
}

void Dino::draw(sf::RenderWindow& window) {
    // Visual indicator for crouching (slightly different color)
    if (isCrouching) {
        body.setFillColor(sf::Color(50, 50, 50)); // Darker gray when crouching
    } else {
        body.setFillColor(sf::Color::Black);
    }
    window.draw(body);
}

sf::FloatRect Dino::getBounds() const {
    return body.getGlobalBounds();
}

void Dino::reset() {
    position = sf::Vector2f(100, GROUND_Y - standingHeight);
    velocityY = 0;
    isGrounded = true;
    isJumping = false;
    isCrouching = false;
    body.setSize(sf::Vector2f(width, standingHeight));
    body.setPosition(position);
    body.setFillColor(sf::Color::Black);
}