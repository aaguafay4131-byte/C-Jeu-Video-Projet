#include "Game.hpp"
#include <iostream>

Game::Game() 
    : window(sf::VideoMode(800, 600), "Dino Game"), 
      dino(), // No arguments now
      ui(),
      clock(),
      score(0),
      highScore(0),
      gameSpeed(5.f),
      isGameOver(false)
{
    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (isGameOver) {
                if (event.key.code == sf::Keyboard::Space) {
                    resetGame();
                }
            } else {
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Up) {
                    dino.jump();
                }
                if (event.key.code == sf::Keyboard::Down) {
                    dino.crouch(true);
                }
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Down) {
                dino.crouch(false);
            }
        }
    }
}

void Game::update() {
    if (isGameOver) return;

    float deltaTime = clock.restart().asSeconds();
    
    // Update Dino (pass delta time)
    dino.update(deltaTime);

    // Simple Obstacle Spawning Logic (Placeholder)
    // You likely have an obstacle manager or vector in your real code
    // For now, assuming you have a way to update obstacles...
    // If you haven't updated Obstacle yet, this part might need adjustment based on your actual Obstacle class
    
    // Example: Move obstacles and check collision
    // Note: You need to adapt this loop to however you store your obstacles (vector? single object?)
    // Assuming you have a std::vector<Obstacle> obstacles; in Game.hpp for this example:
    
    /* 
    for (auto& obs : obstacles) {
        obs.update(gameSpeed * deltaTime);
        if (dino.getBounds().intersects(obs.getBounds())) {
            // Handle Collision
            // Since Dino doesn't have lives anymore in our simple version, 
            // we'll just trigger game over or handle it in UI
            isGameOver = true; 
        }
    }
    */
   
   // TODO: You need to uncomment and adapt the obstacle logic above based on your actual Game.hpp structure.
   // The errors showed you were calling methods like getLives() on Dino which we removed.
   // In this simple version, let's assume 1 hit = Game Over.
   
   // Update UI
   ui.update(score, highScore, isGameOver ? 0 : 3); // Just displaying 3 hearts for now
}

void Game::render() {
    window.clear(sf::Color::White);

    // Draw Background (if you added it)
    // window.draw(backgroundSprite);

    dino.draw(window);
    
    // Draw Obstacles
    // for (const auto& obs : obstacles) obs.draw(window);

    ui.draw(window);

    if (isGameOver) {
        // Draw Game Over Text
        sf::Font font;
        if (font.loadFromFile("assets/font.ttf")) { // Ensure you have a font
            sf::Text text("GAME OVER - Press Space", font, 30);
            text.setPosition(200, 300);
            text.setFillColor(sf::Color::Black);
            window.draw(text);
        }
    }

    window.display();
}

void Game::resetGame() {
    score = 0;
    isGameOver = false;
    dino = Dino(); // Reset dino by creating a new one
    // Clear obstacles vector here
}