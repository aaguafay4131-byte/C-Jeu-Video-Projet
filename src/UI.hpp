#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class UI {
public:
    UI();
    void update(int score, int highScore, int lives);
    void draw(sf::RenderWindow& window) const;

private:
    void updateHearts(int lives);
    void createHeart(float x, float y, bool full);

    sf::Font font;
    sf::Text scoreText;
    sf::Text highScoreText;
    std::vector<sf::CircleShape> hearts;
};