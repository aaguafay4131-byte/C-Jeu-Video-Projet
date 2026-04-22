#include "Game.hpp"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game() : window(sf::VideoMode(800, 600), "Dino Runner - Survive!"),
               currentState(GameState::MENU),
               score(0),
               gameTimer(60.0f),
               countdownTimer(0),
               countdownValue(3),
               isGameRunning(false),
               groundObstacleTimer(0),
               arrowTimer(0),
               groundSpawnInterval(1.0f),
               arrowSpawnInterval(1.0f),
               gameSpeed(300.0f),
               speedMultiplier(1.0f),
               targetSpeedMultiplier(1.0f),
               speedTransitionSpeed(0.5f),
               gameOverTimer(0),
               timeSinceLastGroundObstacle(0),
               timeSinceLastArrow(0),
               heartUI(font),
               menu(font),
               aboutScreen(font),
               background("resources/background.jpg") {  // Add this line
    
    window.setFramerateLimit(60);
    srand(time(nullptr));
    
    if (!font.loadFromFile("resources/fonts/font.ttf")) {
        std::cerr << "Warning: Could not load font file" << std::endl;
    }
    
    // Setup UI Text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10, 10);
    scoreText.setString("Score: 0");
    
    speedText.setFont(font);
    speedText.setCharacterSize(18);
    speedText.setFillColor(sf::Color::Black);
    speedText.setPosition(680, 10);
    speedText.setString("Speed: x1.00");
    
    timerText.setFont(font);
    timerText.setCharacterSize(28);
    timerText.setFillColor(sf::Color::Red);
    timerText.setPosition(350, 10);
    timerText.setString("Time: 60");
    
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(300, 250);
    
    instructionText.setFont(font);
    instructionText.setCharacterSize(16);
    instructionText.setFillColor(sf::Color::Black);
    instructionText.setString("SPACE to jump | DOWN to crouch | ESC to menu");
    instructionText.setPosition(250, 560);
    
    heartUI.setPosition(10, 45);
    
    ground.setSize(sf::Vector2f(800, 10));
    ground.setFillColor(sf::Color::Black);
    ground.setPosition(0, 500);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processInput();
        update(deltaTime);
        render();
    }
}

void Game::processInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                if (currentState == GameState::PLAYING || currentState == GameState::COUNTDOWN) {
                    currentState = GameState::MENU;
                    resetGame();
                } else if (currentState == GameState::ABOUT) {
                    currentState = GameState::MENU;
                } else if (currentState == GameState::MENU) {
                    window.close();
                }
            }
            
            if (currentState == GameState::MENU) {
                if (event.key.code == sf::Keyboard::Up) {
                    menu.moveUp();
                } else if (event.key.code == sf::Keyboard::Down) {
                    menu.moveDown();
                } else if (event.key.code == sf::Keyboard::Enter) {
                    int selected = menu.getSelectedIndex();
                    if (selected == 0) {
                        startGame();
                    } else if (selected == 1) {
                        currentState = GameState::ABOUT;
                    } else if (selected == 2) {
                        window.close();
                    }
                }
            }
            
            if (currentState == GameState::PLAYING) {
                if (event.key.code == sf::Keyboard::Space) {
                    dino.jump();
                } else if (event.key.code == sf::Keyboard::Down) {
                    dino.crouch();
                } else if (event.key.code == sf::Keyboard::R) {
                    resetGame();
                    startGame();
                }
            }
            
            if (currentState == GameState::GAME_OVER) {
                if (event.key.code == sf::Keyboard::Enter) {
                    currentState = GameState::MENU;
                    resetGame();
                }
            }
        }
        
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Down) {
                if (currentState == GameState::PLAYING) {
                    dino.stand();
                }
            }
        }
    }
}

void Game::startGame() {
    resetGame();
    currentState = GameState::COUNTDOWN;
    countdownTimer = 3.0f;
    countdownValue = 3;
    isGameRunning = false;
}

void Game::update(float deltaTime) {
    if (currentState == GameState::COUNTDOWN) {
        countdownTimer -= deltaTime;
        if (countdownTimer <= 0) {
            countdownValue--;
            if (countdownValue > 0) {
                countdownTimer = 1.0f;
            } else {
                currentState = GameState::PLAYING;
                isGameRunning = true;
            }
        }
    }
    
    if (currentState == GameState::PLAYING && isGameRunning) {
        gameTimer -= deltaTime;
        timerText.setString("Time: " + std::to_string((int)gameTimer));
        
        if (gameTimer <= 0) {
            gameOverText.setString("TEMPS ECOULE!");
            gameOverText.setPosition(300, 250);
            currentState = GameState::GAME_OVER;
            return;
        }
        
        dino.update(deltaTime);
        
        for (auto& obstacle : groundObstacles) {
            obstacle.update(deltaTime);
        }
        for (auto& arrow : arrows) {
            arrow.update(deltaTime);
        }
        
        groundObstacles.erase(std::remove_if(groundObstacles.begin(), groundObstacles.end(),
            [](const GroundObstacle& obstacle) { return obstacle.isOffScreen(); }),
            groundObstacles.end());
        
        arrows.erase(std::remove_if(arrows.begin(), arrows.end(),
            [](const Arrow& arrow) { return arrow.isOffScreen(); }),
            arrows.end());
        
        timeSinceLastGroundObstacle += deltaTime;
        timeSinceLastArrow += deltaTime;
        
        float newTargetMultiplier = 1.0f + (score / 10000.0f);
        newTargetMultiplier = std::min(newTargetMultiplier, 2.0f);
        
        if (targetSpeedMultiplier < newTargetMultiplier) {
            targetSpeedMultiplier = newTargetMultiplier;
        }
        
        float speedDiff = targetSpeedMultiplier - speedMultiplier;
        if (std::abs(speedDiff) > 0.001f) {
            speedMultiplier += speedDiff * deltaTime * speedTransitionSpeed;
            gameSpeed = 300.0f * speedMultiplier;
            
            for (auto& obstacle : groundObstacles) {
                obstacle.setSpeed(gameSpeed);
            }
            for (auto& arrow : arrows) {
                arrow.setSpeed(gameSpeed);
            }
            
            char speedBuffer[20];
            sprintf(speedBuffer, "Speed: x%.2f", speedMultiplier);
            speedText.setString(speedBuffer);
        }
        
        groundObstacleTimer += deltaTime;
        arrowTimer += deltaTime;
        
        float groundSpawnChance = 15.0f + (speedMultiplier - 1.0f) * 5.0f;
        float arrowSpawnChance = 12.0f + (speedMultiplier - 1.0f) * 4.0f;
        
        if (groundObstacleTimer >= 0.5f) {
            if (timeSinceLastGroundObstacle >= 1.2f) {
                int chance = rand() % 100;
                if (chance < groundSpawnChance) {
                    if (isSafeToSpawnGroundObstacle()) {
                        spawnGroundObstacle();
                        timeSinceLastGroundObstacle = 0;
                    }
                }
            }
            groundObstacleTimer = 0;
        }
        
        if (arrowTimer >= 0.5f) {
            if (timeSinceLastArrow >= 1.5f) {
                int chance = rand() % 100;
                if (chance < arrowSpawnChance) {
                    if (isSafeToSpawnArrow()) {
                        spawnArrow();
                        timeSinceLastArrow = 0;
                    }
                }
            }
            arrowTimer = 0;
        }
        
        updateScore(deltaTime);
        checkCollisions();
    }
}

bool Game::isSafeToSpawnGroundObstacle() {
    const float SPAWN_X = 800.0f;
    const float MIN_HORIZONTAL_DISTANCE = 250.0f;
    
    if (groundObstacles.size() >= 3) return false;
    
    for (const auto& obstacle : groundObstacles) {
        float obstacleX = obstacle.getBounds().left;
        if (std::abs(SPAWN_X - obstacleX) < MIN_HORIZONTAL_DISTANCE) return false;
    }
    
    for (const auto& arrow : arrows) {
        float arrowX = arrow.getBounds().left;
        if (std::abs(SPAWN_X - arrowX) < 300.0f) return false;
        
        float arrowY = arrow.getBounds().top;
        if (arrowY > 450.0f && std::abs(SPAWN_X - arrowX) < 350.0f) return false;
    }
    
    return true;
}

bool Game::isSafeToSpawnArrow() {
    const float SPAWN_X = 800.0f;
    const float MIN_HORIZONTAL_DISTANCE = 280.0f;
    
    if (arrows.size() >= 2) return false;
    
    for (const auto& arrow : arrows) {
        float arrowX = arrow.getBounds().left;
        if (std::abs(SPAWN_X - arrowX) < MIN_HORIZONTAL_DISTANCE) return false;
    }
    
    for (const auto& obstacle : groundObstacles) {
        float obstacleX = obstacle.getBounds().left;
        if (std::abs(SPAWN_X - obstacleX) < 280.0f) return false;
        
        float obstacleWidth = obstacle.getBounds().width;
        if (obstacleWidth > 25.0f && std::abs(SPAWN_X - obstacleX) < 320.0f) return false;
    }
    
    return true;
}

void Game::spawnGroundObstacle() {
    groundObstacles.emplace_back(800);
    groundObstacles.back().setSpeed(gameSpeed);
}

void Game::spawnArrow() {
    arrows.emplace_back(800);
    arrows.back().setSpeed(gameSpeed);
}

void Game::updateScore(float deltaTime) {
    int pointsPerSecond = 100 * (1 + (speedMultiplier - 1.0f) * 0.5f);
    score += static_cast<int>(pointsPerSecond * deltaTime);
    scoreText.setString("Score: " + std::to_string(score));
}

void Game::checkCollisions() {
    sf::FloatRect dinoBounds = dino.getBounds();
    
    for (const auto& obstacle : groundObstacles) {
        if (dinoBounds.intersects(obstacle.getBounds())) {
            dino.takeDamage();
            const_cast<GroundObstacle&>(obstacle).reset(-100);
            heartUI.update(dino.getHealth());
            
            if (dino.getHealth() <= 0) {
                currentState = GameState::GAME_OVER;
                gameOverText.setString("GAME OVER - VOUS ETES MORT!");
                gameOverText.setPosition(250, 250);
            }
            break;
        }
    }
    
    for (const auto& arrow : arrows) {
        if (dinoBounds.intersects(arrow.getBounds())) {
            dino.takeDamage();
            const_cast<Arrow&>(arrow).reset(-100);
            heartUI.update(dino.getHealth());
            
            if (dino.getHealth() <= 0) {
                currentState = GameState::GAME_OVER;
                gameOverText.setString("GAME OVER - VOUS ETES MORT!");
                gameOverText.setPosition(250, 250);
            }
            break;
        }
    }
}

void Game::resetGame() {
    score = 0;
    gameTimer = 60.0f;
    groundObstacles.clear();
    arrows.clear();
    dino.reset();
    heartUI.update(3);
    groundObstacleTimer = 0;
    arrowTimer = 0;
    timeSinceLastGroundObstacle = 0;
    timeSinceLastArrow = 0;
    gameSpeed = 300.0f;
    speedMultiplier = 1.0f;
    targetSpeedMultiplier = 1.0f;
    scoreText.setString("Score: 0");
    speedText.setString("Speed: x1.00");
    timerText.setString("Time: 60");
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(300, 250);
    isGameRunning = false;
}

void Game::render() {
    window.clear();
    
    // Draw background first (behind everything)
    if (currentState == GameState::PLAYING || currentState == GameState::COUNTDOWN || currentState == GameState::GAME_OVER) {
        background.draw(window);
    }
    
    if (currentState == GameState::MENU) {
        menu.draw(window);
    } 
    else if (currentState == GameState::ABOUT) {
        aboutScreen.draw(window);
    }
    else if (currentState == GameState::COUNTDOWN) {
        dino.draw(window);
        for (auto& obstacle : groundObstacles) obstacle.draw(window);
        for (auto& arrow : arrows) arrow.draw(window);
        window.draw(ground);
        window.draw(scoreText);
        window.draw(speedText);
        window.draw(timerText);
        heartUI.draw(window);
        
        sf::Text countdownText;
        countdownText.setFont(font);
        countdownText.setCharacterSize(100);
        countdownText.setFillColor(sf::Color::Red);
        countdownText.setString(std::to_string(countdownValue));
        countdownText.setPosition(380, 250);
        window.draw(countdownText);
        
        sf::Text getReadyText;
        getReadyText.setFont(font);
        getReadyText.setCharacterSize(30);
        getReadyText.setFillColor(sf::Color::Black);
        getReadyText.setString("Preparez-vous!");
        getReadyText.setPosition(330, 180);
        window.draw(getReadyText);
    }
    else if (currentState == GameState::PLAYING) {
        dino.draw(window);
        for (auto& obstacle : groundObstacles) obstacle.draw(window);
        for (auto& arrow : arrows) arrow.draw(window);
        window.draw(ground);
        window.draw(scoreText);
        window.draw(speedText);
        window.draw(timerText);
        heartUI.draw(window);
        window.draw(instructionText);
    }
    else if (currentState == GameState::GAME_OVER) {
        dino.draw(window);
        for (auto& obstacle : groundObstacles) obstacle.draw(window);
        for (auto& arrow : arrows) arrow.draw(window);
        window.draw(ground);
        window.draw(scoreText);
        window.draw(timerText);
        heartUI.draw(window);
        
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(800, 600));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);
        
        sf::Text finalScoreText;
        finalScoreText.setFont(font);
        finalScoreText.setCharacterSize(30);
        finalScoreText.setFillColor(sf::Color::White);
        finalScoreText.setString("Score Final: " + std::to_string(score));
        finalScoreText.setPosition(320, 320);
        window.draw(finalScoreText);
        window.draw(gameOverText);
        
        sf::Text restartText;
        restartText.setFont(font);
        restartText.setCharacterSize(20);
        restartText.setFillColor(sf::Color::White);
        restartText.setString("Appuyez sur ENTER pour retourner au menu");
        restartText.setPosition(280, 400);
        window.draw(restartText);
    }
    
    window.display();
}