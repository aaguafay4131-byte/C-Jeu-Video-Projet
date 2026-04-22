#include "Menu.hpp"

Menu::Menu(sf::Font& gameFont) : font(gameFont), selectedIndex(0) {
    menuItems = {"Jouer", "A propos", "Quitter"};
    
    background.setSize(sf::Vector2f(800, 600));
    background.setFillColor(sf::Color(50, 50, 80));
    
    title.setFont(font);
    title.setString("DINO RUNNER");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color(255, 215, 0));
    title.setPosition(250, 100);
    
    float yPos = 250;
    for (int i = 0; i < menuItems.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(menuItems[i]);
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::White);
        text.setPosition(350, yPos + (i * 70));
        menuTexts.push_back(text);
    }
    
    menuTexts[0].setFillColor(sf::Color::Yellow);
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(background);
    window.draw(title);
    for (auto& text : menuTexts) {
        window.draw(text);
    }
}

void Menu::moveUp() {
    if (selectedIndex > 0) {
        menuTexts[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        menuTexts[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}

void Menu::moveDown() {
    if (selectedIndex < menuItems.size() - 1) {
        menuTexts[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        menuTexts[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}

int Menu::getSelectedIndex() const {
    return selectedIndex;
}