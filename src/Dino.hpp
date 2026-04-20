#ifndef DINO_HPP
#define DINO_HPP

#include <SFML/Graphics.hpp>

class Dino {
public:
    Dino();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void jump();
    void crouch(bool isCrouching);

private:
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    
    float m_velocity;
    float m_gravity;
    float m_jumpStrength;
    bool m_isJumping;
    bool m_isCrouching;
    
    float m_width;
    float m_height;

    float m_groundY;
};

#endif