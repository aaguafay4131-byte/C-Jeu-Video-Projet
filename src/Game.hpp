#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Dino.hpp"
#include "GroundObstacle.hpp"
#include "Bird.hpp"
#include "GameState.hpp"

class Game {
private:
    sf::RenderWindow window;
    Dino dino;
    std::vector<GroundObstacle> groundObstacles;
    std::vector<Bird> birds;
    GameState currentState;
    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text instructionText;
    sf::Text speedText;  // New: Display current speed multiplier
    int score;
    float groundObstacleTimer;
    float birdTimer;
    float groundSpawnInterval;
    float birdSpawnInterval;
    sf::RectangleShape ground;
    float gameSpeed;
    float speedMultiplier;
    float targetSpeedMultiplier;  // New: Target multiplier for smooth transition
    float speedTransitionSpeed;   // New: How fast the speed changes
    
    // Anti-clustering variables
    float lastGroundObstacleX;
    float lastBirdX;
    float timeSinceLastGroundObstacle;
    float timeSinceLastBird;
    
    // Helper methods
    bool isSafeToSpawnGroundObstacle();
    bool isSafeToSpawnBird();

    void processInput();
    void update(float deltaTime);
    void render();
    void spawnGroundObstacle();
    void spawnBird();
    void updateScore(float deltaTime);
    void checkCollisions();
    void resetGame();
    void increaseDifficulty();

public:
    Game();
    void run();
};