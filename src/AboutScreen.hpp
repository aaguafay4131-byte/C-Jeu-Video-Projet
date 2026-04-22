#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class AboutScreen {
private:
    sf::Font& font;
    sf::Text title;
    std::vector<sf::Text> aboutText;
    sf::Text backInstruction;
    sf::RectangleShape background;
    
public:
    AboutScreen(sf::Font& gameFont);
    void draw(sf::RenderWindow& window);
};