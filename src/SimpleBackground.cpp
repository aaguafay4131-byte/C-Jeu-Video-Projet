#include "SimpleBackground.hpp"
#include <iostream>

SimpleBackground::SimpleBackground(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load background texture: " << texturePath << std::endl;
        // Create a simple fallback background
        sf::Image image;
        image.create(800, 600, sf::Color(135, 206, 235)); // Sky blue
        texture.loadFromImage(image);
    }
    
    sprite.setTexture(texture);
    
    // Scale to fit screen if needed
    float scaleX = 800.0f / texture.getSize().x;
    float scaleY = 600.0f / texture.getSize().y;
    sprite.setScale(scaleX, scaleY);
    sprite.setPosition(0, 0);
}

void SimpleBackground::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}