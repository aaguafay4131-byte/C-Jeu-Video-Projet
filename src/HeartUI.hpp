#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class HeartUI {
private:
    std::vector<sf::RectangleShape> hearts;
    sf::Text healthText;
    sf::Vector2f position;
    float heartSize;
    
public:
    HeartUI(sf::Font& gameFont);
    void update(int health);
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
};