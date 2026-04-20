#include "Dino.hpp"
#include <iostream>

Dino::Dino() 
    : m_velocity(0.f), 
      m_gravity(800.f), 
      m_jumpStrength(-450.f), 
      m_isJumping(false),
      m_isCrouching(false),
      m_groundY(400.f) 
{
    // Try to load the texture from file
    if (!m_texture.loadFromFile("assets/sprites/dino.png")) {
        std::cerr << "Warning: Could not find assets/sprites/dino.png. Using fallback color." << std::endl;
        
        // Create a fallback green image manually (compatible with all SFML versions)
        sf::Image img;
        img.create(50, 60, sf::Color::Green); // Creates a 50x60 green image directly
        m_texture.loadFromImage(img);
    }

    m_sprite.setTexture(m_texture);
    m_sprite.setPosition(100.f, m_groundY);

    m_sprite.setScale(0.01f, 0.01f); 
}

void Dino::update(float deltaTime) {
    if (m_isJumping) {
        m_velocity += m_gravity * deltaTime;
        m_sprite.move(0.f, m_velocity * deltaTime);

        if (m_sprite.getPosition().y >= m_groundY) {
            m_sprite.setPosition(m_sprite.getPosition().x, m_groundY);
            m_velocity = 0.f;
            m_isJumping = false;
        }
    }
    
    if (m_isCrouching && !m_isJumping) {
        m_sprite.setScale(1.0f, 0.6f);
    } else {
        m_sprite.setScale(1.0f, 1.0f);
    }
}

void Dino::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}

sf::FloatRect Dino::getBounds() const {
    return m_sprite.getGlobalBounds();
}

void Dino::jump() {
    if (!m_isJumping) {
        m_velocity = m_jumpStrength;
        m_isJumping = true;
    }
}

void Dino::crouch(bool isCrouching) {
    m_isCrouching = isCrouching;
}