#include "HeartUI.hpp"

HeartUI::HeartUI(sf::Font& gameFont) {
    heartSize = 25.0f;
    position = sf::Vector2f(10, 45);
    
    for (int i = 0; i < 3; i++) {
        sf::RectangleShape heart;
        heart.setSize(sf::Vector2f(heartSize, heartSize));
        heart.setPosition(position.x + (i * (heartSize + 5)), position.y);
        heart.setFillColor(sf::Color::Red);
        hearts.push_back(heart);
    }
    
    healthText.setFont(gameFont);
    healthText.setCharacterSize(16);
    healthText.setFillColor(sf::Color::Black);
    healthText.setPosition(position.x, position.y - 18);
    healthText.setString("LIVES");
}

void HeartUI::update(int health) {
    for (int i = 0; i < hearts.size(); i++) {
        if (i < health) {
            hearts[i].setFillColor(sf::Color::Red);
        } else {
            hearts[i].setFillColor(sf::Color(80, 80, 80));
        }
    }
}

void HeartUI::draw(sf::RenderWindow& window) {
    window.draw(healthText);
    for (auto& heart : hearts) {
        window.draw(heart);
    }
}

void HeartUI::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    healthText.setPosition(x, y - 18);
    for (int i = 0; i < hearts.size(); i++) {
        hearts[i].setPosition(x + (i * (heartSize + 5)), y);
    }
}