#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    sf::Font& font;
    std::vector<std::string> menuItems;
    std::vector<sf::Text> menuTexts;
    int selectedIndex;
    sf::Text title;
    sf::RectangleShape background;
    
public:
    Menu(sf::Font& gameFont);
    void draw(sf::RenderWindow& window);
    void moveUp();
    void moveDown();
    int getSelectedIndex() const;
};