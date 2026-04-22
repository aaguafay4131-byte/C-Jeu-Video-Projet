#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class DinoState {
    OK,      // 3 hearts - normal
    HURT,    // 1-2 hearts - damaged
    DEAD     // 0 hearts - game over
};

enum class AnimationState {
    IDLE,
    RUNNING,
    JUMPING,
    CROUCHING,
    HURT,
    DEAD
};

class Dino {
private:
    // Sprite and animation
    sf::Texture spriteSheet;
    sf::Sprite sprite;
    
    // Animation frames (texture rectangles)
    std::vector<sf::IntRect> runFrames;
    sf::IntRect jumpFrame;
    sf::IntRect idleFrame;
    sf::IntRect crouchFullFrame;  // Changed from crouchFrames vector
    sf::IntRect hurtFrame;
    sf::IntRect deadFrame;
    
    int currentFrame;
    float animationTimer;
    float animationSpeed;
    AnimationState currentAnimation;
    
    // Physics
    sf::Vector2f position;
    float velocityY;
    bool isGrounded;
    bool isJumping;
    bool isCrouching;
    
    // Health
    DinoState state;
    int health;
    float invincibilityTimer;
    float invincibilityDuration;
    bool isInvincible;
    
    // Size variables
    float standingHeight;
    float crouchHeight;
    float width;
    
    const float GRAVITY = 1200.0f;
    const float JUMP_FORCE = -450.0f;
    const float GROUND_Y = 500.0f;

    // Helper methods
    void loadSpriteSheet();
    void createFallbackSprite();
    void setAnimation(AnimationState newAnimation);
    void updateAnimation(float deltaTime);

public:
    Dino();
    void update(float deltaTime);
    void jump();
    void crouch();
    void stand();
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void reset();
    void takeDamage();
    void heal();
    
    // Getters
    bool isCrouchingState() const;
    bool isJumpingState() const;
    DinoState getState() const;
    int getHealth() const;
    bool isInvincibleState() const;
};