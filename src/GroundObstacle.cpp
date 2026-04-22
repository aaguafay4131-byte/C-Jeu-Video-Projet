#include "GroundObstacle.hpp"
#include <cstdlib>
#include <iostream>

GroundObstacle::GroundObstacle(float startX) {
    speed = 300.0f;
    
    // Load the cactus sprite sheet
    if (!spriteSheet.loadFromFile("resources/sprites/CactiSprite.png")) {
        std::cerr << "Failed to load cactus sprite sheet! Using fallback rectangles." << std::endl;
        
        // Fallback to rectangles if sprite sheet doesn't load
        cactusType = rand() % 2;
        
        if (cactusType == 0) {
            width = 50.0f;
            height = 70.0f;
        } else {
            width = 65.0f;
            height = 90.0f;
        }
        
        position = sf::Vector2f(startX, 500 - height);
    } else {
        // Sprite sheet loaded successfully
        int frameWidth = 423;
        int frameHeight = 295;
        
        cactusType = rand() % 2;
        
        if (cactusType == 0) {
            // Small cactus
            sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
            width = 55.0f;   // Slightly reduced width
            height = 75.0f;  // Reduced height
        } else {
            // Large cactus
            sprite.setTextureRect(sf::IntRect(frameWidth, 0, frameWidth, frameHeight));
            width = 70.0f;   // Slightly reduced width
            height = 95.0f;  // Reduced height
        }
        
        sprite.setTexture(spriteSheet);
        
        // Scale the sprite
        float scaleX = width / frameWidth;
        float scaleY = height / frameHeight;
        sprite.setScale(scaleX, scaleY);
        
        std::cout << "Cactus loaded - Type: " << (cactusType == 0 ? "Small" : "Large") 
                  << " | Size: " << width << "x" << height << std::endl;
    }
    
    position = sf::Vector2f(startX, 500 - height);
    sprite.setPosition(position);
}

void GroundObstacle::update(float deltaTime) {
    position.x -= speed * deltaTime;
    sprite.setPosition(position);
}

void GroundObstacle::draw(sf::RenderWindow& window) {
    if (sprite.getTexture() != nullptr) {
        window.draw(sprite);
        
        // Optional: Draw hitbox for debugging (uncomment to see)
        
        sf::RectangleShape hitbox(sf::Vector2f(getBounds().width, getBounds().height));
        hitbox.setPosition(getBounds().left, getBounds().top);
        hitbox.setFillColor(sf::Color(255, 0, 0, 100));
        window.draw(hitbox);
        
    } else {
        // Fallback rectangle drawing
        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setFillColor(sf::Color(34, 139, 34));
        rect.setPosition(position);
        window.draw(rect);
    }
}

sf::FloatRect GroundObstacle::getBounds() const {
    // Get the actual sprite bounds
    sf::FloatRect bounds = sprite.getGlobalBounds();
    
    // Adjust hitbox to be slightly smaller than sprite for fairer gameplay
    // This makes the hitbox more appropriate for the cactus shape
    float hitboxWidth = bounds.width * 0.70f;   // 70% of sprite width
    float hitboxHeight = bounds.height * 0.60f; // 85% of sprite height
    
    // Center the hitbox horizontally
    float hitboxX = bounds.left + (bounds.width - hitboxWidth) / 2;
    // Keep hitbox at bottom of cactus (feet level)
    float hitboxY = bounds.top + (bounds.height - hitboxHeight);
    
    return sf::FloatRect(hitboxX, hitboxY, hitboxWidth, hitboxHeight);
}

bool GroundObstacle::isOffScreen() const {
    return position.x + width < 0;
}

void GroundObstacle::reset(float startX) {
    position.x = startX;
    sprite.setPosition(position);
}

void GroundObstacle::setSpeed(float newSpeed) {
    speed = newSpeed;
}