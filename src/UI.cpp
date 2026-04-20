#include "UI.hpp"
#include <iostream>

UI::UI() {
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Error: Could not load font.ttf!\n";
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(20, 20);

    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color(100, 100, 100));
    highScoreText.setPosition(20, 50);

    updateHearts(3);
}

void UI::updateHearts(int lives) {
    hearts.clear();
    float startX = 20.0f;
    float yPos = 85.0f;
    float spacing = 25.0f;

    for (int i = 0; i < 3; i++) {
        bool isFull = (i < lives);
        createHeart(startX + (i * spacing), yPos, isFull);
    }
}

void UI::createHeart(float x, float y, bool full) {
    float size = 12.0f;
    sf::CircleShape heartCircle(size);
    heartCircle.setPosition(x, y);
    heartCircle.setFillColor(full ? sf::Color(255, 50, 50) : sf::Color(150, 150, 150));
    hearts.push_back(heartCircle);
}

void UI::update(int score, int highScore, int lives) {
    scoreText.setString("Score: " + std::to_string(score));
    highScoreText.setString("High Score: " + std::to_string(highScore));
    updateHearts(lives);
}

void UI::draw(sf::RenderWindow& window) const {
    window.draw(scoreText);
    window.draw(highScoreText);
    for (const auto& heart : hearts) {
        window.draw(heart);
    }
}