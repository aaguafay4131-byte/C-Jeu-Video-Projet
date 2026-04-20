#include "Game.hpp"

auto centerText = [](sf::Text& txt, float x, float y) {
    sf::FloatRect rect = txt.getLocalBounds();
    txt.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
    txt.setPosition(x, y);
};

Game::Game()
    : window(sf::VideoMode(800, 400), "Dino Game"),
      dino(100, 250, GROUND_Y),
      state(GameState::Start),
      spawnTimer(0.0f),
      spawnInterval(1.5f),
      score(0),
      highScore(0),
      gameSpeed(5.0f) {
    
    window.setFramerateLimit(60);
    ground.setSize({800, 2});
    ground.setFillColor(sf::Color::Black);
    ground.setPosition(0, GROUND_Y);
    
    rng.seed(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));

    if (!screenFont.loadFromFile("font.ttf")) {
        std::cerr << "Error: Could not load font.ttf!\n";
    }

    titleText.setFont(screenFont);
    titleText.setString("DINO RUNNER");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color::Black);
    centerText(titleText, 400, 150);

    subtitleText.setFont(screenFont);
    subtitleText.setString("Press SPACE to Start");
    subtitleText.setCharacterSize(24);
    subtitleText.setFillColor(sf::Color(100, 100, 100));
    centerText(subtitleText, 400, 290);

    gameOverText.setFont(screenFont);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color(200, 50, 50));
    centerText(gameOverText, 400, 150);

    finalScoreText.setFont(screenFont);
    finalScoreText.setString("Final Score: 0");
    finalScoreText.setCharacterSize(24);
    finalScoreText.setFillColor(sf::Color::Black);
    centerText(finalScoreText, 400, 220);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        if (state == GameState::Playing) {
            update();
        }
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
            if (state == GameState::Start && event.key.code == sf::Keyboard::Space) {
                state = GameState::Playing;
            } else if (state == GameState::GameOver && event.key.code == sf::Keyboard::Space) {
                resetGame();
                state = GameState::Playing;
            }
        }
    }
    if (state == GameState::Playing) {
        dino.handleInput();
    }
}

void Game::update() {
    dino.update();

    score++;
    if (score > highScore) highScore = score;
    gameSpeed = 5.0f + (score * 0.002f);
    if (gameSpeed > MAX_SPEED) gameSpeed = MAX_SPEED;
    
    int lives = dino.getLives();
    ui.update(score, highScore, lives);

    spawnTimer += 1.0f / 60.0f;
    if (spawnTimer >= spawnInterval) {
        spawnObstacle();
        spawnTimer = 0.0f;
        std::uniform_real_distribution<float> dist(0.8f, 2.2f);
        spawnInterval = dist(rng);
    }

    for (auto& obs : obstacles) {
        obs.update();
    }
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
        [](const Obstacle& o) { return o.isOffScreen(); }), obstacles.end());

    for (const auto& obs : obstacles) {
        if (dino.getGlobalBounds().intersects(obs.getGlobalBounds())) {
            dino.takeHit();
            ui.update(score, highScore, dino.getLives()); // Update hearts immediately
            if (dino.getState() == Dino::State::Mort) {
                state = GameState::GameOver;
            }
            break;
        }
    }
}

void Game::spawnObstacle() {
    std::uniform_int_distribution<int> typeDist(0, 1);
    bool isAerial = (typeDist(rng) == 1);

    if (isAerial) {
        std::uniform_real_distribution<float> heightDist(220.0f, 260.0f);
        float y = heightDist(rng);
        float height = 25.0f;
        obstacles.emplace_back(800.0f, y, 35.0f, height, gameSpeed, true);
    } else {
        float height = 30.0f + (std::rand() % 40);
        float y = GROUND_Y - height;
        obstacles.emplace_back(800.0f, y, 20.0f, height, gameSpeed, false);
    }
}

void Game::resetGame() {
    obstacles.clear();
    spawnTimer = 0.0f;
    spawnInterval = 1.5f;
    state = GameState::Playing;
    score = 0;
    gameSpeed = 5.0f;
    dino.reset();
}

void Game::render() {
    window.clear(sf::Color::White);
    
    if (state == GameState::Start) {
        window.draw(titleText);
        window.draw(subtitleText);
    } 
    else if (state == GameState::GameOver) {
        finalScoreText.setString("Final Score: " + std::to_string(score));
        sf::FloatRect scoreRect = finalScoreText.getLocalBounds();
        finalScoreText.setOrigin(scoreRect.left + scoreRect.width / 2.0f, scoreRect.top + scoreRect.height / 2.0f);
        finalScoreText.setPosition(400, 220);

        subtitleText.setString("Press SPACE to Restart");
        sf::FloatRect subRect = subtitleText.getLocalBounds();
        subtitleText.setOrigin(subRect.left + subRect.width / 2.0f, subRect.top + subRect.height / 2.0f);
        subtitleText.setPosition(400, 290);

        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(subtitleText);
    } 
    else {
        window.draw(ground);
        for (const auto& obs : obstacles) {
            obs.draw(window);
        }
        dino.draw(window);
        ui.draw(window);
    }
    
    window.display();
}