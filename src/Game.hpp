#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Dino.hpp"
#include "UI.hpp"
// Include Obstacle if you have it, otherwise comment out
// #include "Obstacle.hpp" 

class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update();
    void render();
    void resetGame();

    sf::RenderWindow window;
    Dino dino;
    UI ui;
    
    // Missing members added here:
    sf::Clock clock;      // SFML Clock for delta time
    bool isGameOver;      // Game state flag
    
    int score;
    int highScore;
    float gameSpeed;
    
    // If you have obstacles, add them here later
    // std::vector<Obstacle> obstacles;
};

#endif