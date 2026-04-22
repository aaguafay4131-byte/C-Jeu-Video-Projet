#pragma once
#include <SFML/Graphics.hpp>

class SimpleBackground {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    SimpleBackground(const std::string& texturePath);
    void draw(sf::RenderWindow& window);
};