#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <string>
#include <iostream>

#include "Dino.hpp"
#include "Obstacle.hpp"
#include "UI.hpp"

enum class GameState { Start, Playing, GameOver };

class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update();
    void spawnObstacle();
    void resetGame();
    void render();

    sf::RenderWindow window;
    Dino dino;
    UI ui;
    sf::RectangleShape ground;
    std::vector<Obstacle> obstacles;
    GameState state;
    float spawnTimer;
    float spawnInterval;
    std::mt19937 rng;

    int score;
    int highScore;
    float gameSpeed;
    static constexpr float GROUND_Y = 300.0f;
    static constexpr float MAX_SPEED = 12.0f;

    sf::Font screenFont;
    sf::Text titleText;
    sf::Text subtitleText;
    sf::Text gameOverText;
    sf::Text finalScoreText;
};