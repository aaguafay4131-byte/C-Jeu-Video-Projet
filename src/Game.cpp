#include "Game.hpp"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game() : window(sf::VideoMode(800, 600), "Dino Game - Flying Birds!"),
               currentState(GameState::PLAYING),
               score(0),
               groundObstacleTimer(0),
               birdTimer(0),
               groundSpawnInterval(1.0f),
               birdSpawnInterval(1.0f),
               gameSpeed(300.0f),
               speedMultiplier(1.0f),
               targetSpeedMultiplier(1.0f),
               speedTransitionSpeed(0.5f),
               lastGroundObstacleX(0),
               lastBirdX(0),
               timeSinceLastGroundObstacle(0),
               timeSinceLastBird(0) {
    
    window.setFramerateLimit(60);
    srand(time(nullptr));
    
    // Load font
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Warning: Could not load font file" << std::endl;
    }
    
    // Setup text
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10, 10);
    scoreText.setString("Score: 0");
    
    speedText.setFont(font);
    speedText.setCharacterSize(20);
    speedText.setFillColor(sf::Color::Black);
    speedText.setPosition(10, 50);
    speedText.setString("Speed: x1.00");
    
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(300, 250);
    
    instructionText.setFont(font);
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color::Black);
    instructionText.setString("SPACE to jump | DOWN to crouch | R to restart | Avoid birds & cacti!");
    instructionText.setPosition(180, 550);
    
    // Setup ground
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
            if (event.key.code == sf::Keyboard::Space) {
                if (currentState == GameState::PLAYING) {
                    dino.jump();
                } else if (currentState == GameState::GAME_OVER) {
                    resetGame();
                }
            }
            
            if (event.key.code == sf::Keyboard::R) {
                resetGame();
            }
            
            // Crouch with DOWN arrow key
            if (event.key.code == sf::Keyboard::Down) {
                if (currentState == GameState::PLAYING) {
                    dino.crouch();
                }
            }
        }
        
        // Handle key release for crouching
        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Down) {
                if (currentState == GameState::PLAYING) {
                    dino.stand();
                }
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (currentState == GameState::PLAYING) {
        dino.update(deltaTime);
        
        // Update ground obstacles
        for (auto& obstacle : groundObstacles) {
            obstacle.update(deltaTime);
        }
        
        // Update birds
        for (auto& bird : birds) {
            bird.update(deltaTime);
        }
        
        // Remove off-screen ground obstacles
        groundObstacles.erase(std::remove_if(groundObstacles.begin(), groundObstacles.end(),
            [](const GroundObstacle& obstacle) { return obstacle.isOffScreen(); }),
            groundObstacles.end());
        
        // Remove off-screen birds
        birds.erase(std::remove_if(birds.begin(), birds.end(),
            [](const Bird& bird) { return bird.isOffScreen(); }),
            birds.end());
        
        // Update timers for anti-clustering
        timeSinceLastGroundObstacle += deltaTime;
        timeSinceLastBird += deltaTime;
        
        // =============================================
        // SMOOTH SPEED TRANSITION SYSTEM
        // =============================================
        
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
            for (auto& bird : birds) {
                bird.setSpeed(gameSpeed);
            }
            
            char speedBuffer[20];
            sprintf(speedBuffer, "Speed: x%.2f", speedMultiplier);
            speedText.setString(speedBuffer);
        }
        
        // =============================================
        // OBSTACLE SPAWNING WITH PROPER POSITION CHECKING
        // =============================================
        
        groundObstacleTimer += deltaTime;
        birdTimer += deltaTime;
        
        float groundSpawnChance = 15.0f + (speedMultiplier - 1.0f) * 5.0f;
        float birdSpawnChance = 12.0f + (speedMultiplier - 1.0f) * 4.0f;
        
        // Check for ground obstacle spawning
        if (groundObstacleTimer >= 0.5f) {
            if (timeSinceLastGroundObstacle >= 1.2f) {
                int chance = rand() % 100;
                if (chance < groundSpawnChance) {
                    if (isSafeToSpawnGroundObstacle()) {
                        spawnGroundObstacle();
                        timeSinceLastGroundObstacle = 0;
                        std::cout << "Spawned ground obstacle at score: " << score << std::endl; // Debug
                    }
                }
            }
            groundObstacleTimer = 0;
        }
        
        // Check for bird spawning
        if (birdTimer >= 0.5f) {
            if (timeSinceLastBird >= 1.5f) {
                int chance = rand() % 100;
                if (chance < birdSpawnChance) {
                    if (isSafeToSpawnBird()) {
                        spawnBird();
                        timeSinceLastBird = 0;
                        std::cout << "Spawned bird at score: " << score << std::endl; // Debug
                    }
                }
            }
            birdTimer = 0;
        }
        
        updateScore(deltaTime);
        checkCollisions();
    }
}

bool Game::isSafeToSpawnGroundObstacle() {
    const float SPAWN_X = 800.0f;
    const float MIN_HORIZONTAL_DISTANCE = 250.0f;  // Minimum pixels between obstacles
    
    // Don't spawn if there are already 3 ground obstacles on screen
    if (groundObstacles.size() >= 3) {
        return false;
    }
    
    // Check all ground obstacles
    for (const auto& obstacle : groundObstacles) {
        float obstacleX = obstacle.getBounds().left;
        float distance = std::abs(SPAWN_X - obstacleX);
        
        if (distance < MIN_HORIZONTAL_DISTANCE) {
            return false;  // Too close to another ground obstacle
        }
    }
    
    // Check all birds - they shouldn't spawn too close to birds either
    for (const auto& bird : birds) {
        float birdX = bird.getBounds().left;
        float distance = std::abs(SPAWN_X - birdX);
        
        // If bird is within 300 pixels, don't spawn ground obstacle
        if (distance < 300.0f) {
            return false;
        }
        
        // Extra check for low-flying birds (more dangerous overlap)
        float birdY = bird.getBounds().top;
        if (birdY > 450.0f && distance < 350.0f) {
            return false;  // Low bird needs even more space
        }
    }
    
    return true;
}

bool Game::isSafeToSpawnBird() {
    const float SPAWN_X = 800.0f;
    const float MIN_HORIZONTAL_DISTANCE = 280.0f;  // Minimum pixels between birds
    
    // Don't spawn if there are already 2 birds on screen
    if (birds.size() >= 2) {
        return false;
    }
    
    // Check all existing birds
    for (const auto& bird : birds) {
        float birdX = bird.getBounds().left;
        float distance = std::abs(SPAWN_X - birdX);
        
        if (distance < MIN_HORIZONTAL_DISTANCE) {
            return false;  // Too close to another bird
        }
    }
    
    // Check all ground obstacles
    for (const auto& obstacle : groundObstacles) {
        float obstacleX = obstacle.getBounds().left;
        float distance = std::abs(SPAWN_X - obstacleX);
        
        // If ground obstacle is within 280 pixels, don't spawn bird
        if (distance < 280.0f) {
            return false;
        }
        
        // Extra check for wide cacti
        float obstacleWidth = obstacle.getBounds().width;
        if (obstacleWidth > 25.0f && distance < 320.0f) {
            return false;  // Wide cactus needs more space
        }
    }
    
    return true;
}

void Game::spawnGroundObstacle() {
    groundObstacles.emplace_back(800);
    groundObstacles.back().setSpeed(gameSpeed);
}

void Game::spawnBird() {
    birds.emplace_back(800);
    birds.back().setSpeed(gameSpeed);
}

void Game::updateScore(float deltaTime) {
    int pointsPerSecond = 100 * (1 + (speedMultiplier - 1.0f) * 0.5f);
    score += static_cast<int>(pointsPerSecond * deltaTime);
    scoreText.setString("Score: " + std::to_string(score));
}

void Game::checkCollisions() {
    sf::FloatRect dinoBounds = dino.getBounds();
    
    // Check collisions with ground obstacles
    for (const auto& obstacle : groundObstacles) {
        if (dinoBounds.intersects(obstacle.getBounds())) {
            currentState = GameState::GAME_OVER;
            return;
        }
    }
    
    // Check collisions with birds
    for (const auto& bird : birds) {
        if (dinoBounds.intersects(bird.getBounds())) {
            sf::FloatRect birdBounds = bird.getBounds();
            
            if (dino.isCrouchingState()) {
                float birdBottom = birdBounds.top + birdBounds.height;
                float dinoTop = dinoBounds.top;
                
                if (birdBottom > dinoTop && birdBounds.top < dinoBounds.top + dinoBounds.height) {
                    currentState = GameState::GAME_OVER;
                    return;
                }
            } else {
                currentState = GameState::GAME_OVER;
                return;
            }
        }
    }
}

void Game::resetGame() {
    currentState = GameState::PLAYING;
    score = 0;
    groundObstacles.clear();
    birds.clear();
    dino.reset();
    groundObstacleTimer = 0;
    birdTimer = 0;
    timeSinceLastGroundObstacle = 0;
    timeSinceLastBird = 0;
    gameSpeed = 300.0f;
    speedMultiplier = 1.0f;
    targetSpeedMultiplier = 1.0f;
    scoreText.setString("Score: 0");
    speedText.setString("Speed: x1.00");
}

void Game::render() {
    window.clear(sf::Color(135, 206, 235));
    
    // Draw clouds
    sf::CircleShape cloud1(30);
    cloud1.setFillColor(sf::Color(255, 255, 255, 200));
    cloud1.setPosition(100, 80);
    window.draw(cloud1);
    
    sf::CircleShape cloud2(40);
    cloud2.setFillColor(sf::Color(255, 255, 255, 200));
    cloud2.setPosition(130, 70);
    window.draw(cloud2);
    
    sf::CircleShape cloud3(25);
    cloud3.setFillColor(sf::Color(255, 255, 255, 200));
    cloud3.setPosition(70, 90);
    window.draw(cloud3);
    
    // Draw game objects
    dino.draw(window);
    
    for (auto& obstacle : groundObstacles) {
        obstacle.draw(window);
    }
    
    for (auto& bird : birds) {
        bird.draw(window);
    }
    
    window.draw(ground);
    window.draw(scoreText);
    window.draw(speedText);
    window.draw(instructionText);
    
    // Debug: Visualize spawn safe zone (optional - remove in production)
    #ifdef _DEBUG
    sf::RectangleShape safeZone(sf::Vector2f(300, 600));
    safeZone.setFillColor(sf::Color(255, 0, 0, 30));
    safeZone.setPosition(500, 0);
    window.draw(safeZone);
    #endif
    
    // Show tip
    if (currentState == GameState::PLAYING && score < 200) {
        sf::Text crouchHint;
        crouchHint.setFont(font);
        crouchHint.setCharacterSize(16);
        crouchHint.setFillColor(sf::Color(100, 100, 100));
        crouchHint.setString("Tip: Press DOWN to crouch under low-flying birds!");
        crouchHint.setPosition(250, 520);
        window.draw(crouchHint);
    }
    
    if (currentState == GameState::GAME_OVER) {
        window.draw(gameOverText);
    }
    
    window.display();
}