#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Dino.hpp"
#include "GroundObstacle.hpp"
#include "Arrow.hpp"
#include "HeartUI.hpp"
#include "Menu.hpp"
#include "AboutScreen.hpp"
#include "GameState.hpp"
#include "SimpleBackground.hpp"  // Add this

class Game {
private:
    sf::RenderWindow window;
    Dino dino;
    std::vector<GroundObstacle> groundObstacles;
    std::vector<Arrow> arrows;
    GameState currentState;
    sf::Font font;
    
    // Background
    SimpleBackground background;  // Add this
    
    // UI Elements
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text instructionText;
    sf::Text speedText;
    sf::Text timerText;
    HeartUI heartUI;
    
    // Menu and About
    Menu menu;
    AboutScreen aboutScreen;
    
    // Game variables
    int score;
    float gameTimer;
    float countdownTimer;
    int countdownValue;
    bool isGameRunning;
    float groundObstacleTimer;
    float arrowTimer;
    float groundSpawnInterval;
    float arrowSpawnInterval;
    sf::RectangleShape ground;
    float gameSpeed;
    float speedMultiplier;
    float targetSpeedMultiplier;
    float speedTransitionSpeed;
    float gameOverTimer;
    
    // Anti-clustering variables
    float timeSinceLastGroundObstacle;
    float timeSinceLastArrow;
    
    // Helper methods
    bool isSafeToSpawnGroundObstacle();
    bool isSafeToSpawnArrow();
    void processInput();
    void update(float deltaTime);
    void render();
    void spawnGroundObstacle();
    void spawnArrow();
    void updateScore(float deltaTime);
    void checkCollisions();
    void resetGame();
    void startGame();

public:
    Game();
    void run();
};