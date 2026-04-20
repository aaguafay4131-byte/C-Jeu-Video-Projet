#pragma once
#include <SFML/Graphics.hpp>

class Dino {
public:
    enum class State { OK, Blesse, Mort };

    Dino(float startX, float startY, float groundY);

    void handleInput();
    void update();
    void takeHit();
    void reset();
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getGlobalBounds() const;
    int getLives() const;
    State getState() const;

private:
    sf::RectangleShape shape;
    float velocityY;
    bool isGrounded;
    bool isCrouching;
    float groundLevel;

    State state;
    int hitCount;
    float invincibilityTimer;

    static constexpr float INVINCIBILITY_FRAMES = 45.0f;
    static constexpr int MAX_HITS = 3;
    static constexpr float GRAVITY = 0.6f;
    static constexpr float JUMP_FORCE = -12.0f;
    static constexpr float WIDTH = 40.0f;
    static constexpr float NORMAL_HEIGHT = 50.0f;
    static constexpr float CROUCH_HEIGHT = 25.0f;
};