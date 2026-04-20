#pragma once
#include <SFML/Graphics.hpp>

class Dino {
private:
    sf::RectangleShape body;
    sf::Vector2f position;
    float velocityY;
    bool isGrounded;
    bool isJumping;
    bool isCrouching;
    
    // Size variables for standing and crouching
    float standingHeight;
    float crouchHeight;
    float width;
    
    const float GRAVITY = 1200.0f;
    const float JUMP_FORCE = -450.0f;
    const float GROUND_Y = 500.0f;

public:
    Dino();
    void update(float deltaTime);
    void jump();
    void crouch();
    void stand();
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void reset();
    
    // Getters
    bool isCrouchingState() const { return isCrouching; }
    bool isJumpingState() const { return isJumping; }
};