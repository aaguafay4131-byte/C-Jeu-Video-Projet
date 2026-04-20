#include "Dino.hpp"

Dino::Dino(float startX, float startY, float groundY)
    : velocityY(0.0f), isGrounded(true), isCrouching(false), groundLevel(groundY),
      state(State::OK), hitCount(0), invincibilityTimer(0.0f) {
    shape.setSize({WIDTH, NORMAL_HEIGHT});
    shape.setFillColor(sf::Color::Black);
    shape.setPosition(startX, startY);
}

void Dino::handleInput() {
    if (state == State::Mort) return;

    bool inputCrouch = sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
                       sf::Keyboard::isKeyPressed(sf::Keyboard::S);
    bool wantJump = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

    isCrouching = inputCrouch && isGrounded;

    if (wantJump && isGrounded && !isCrouching) {
        velocityY = JUMP_FORCE;
        isGrounded = false;
        isCrouching = false;
    }
}

void Dino::update() {
    if (invincibilityTimer > 0.0f) {
        invincibilityTimer -= 1.0f;
        if (state == State::Blesse) {
            bool visible = static_cast<int>(invincibilityTimer) % 4 != 0;
            shape.setFillColor(visible ? sf::Color(200, 50, 50) : sf::Color::Transparent);
        }
    } else if (state == State::Blesse) {
        shape.setFillColor(sf::Color(200, 50, 50));
    }

    if (state == State::Mort) return;

    float targetHeight = isCrouching ? CROUCH_HEIGHT : NORMAL_HEIGHT;
    if (shape.getSize().y != targetHeight) {
        float currentBottom = shape.getPosition().y + shape.getSize().y;
        shape.setSize({WIDTH, targetHeight});
        shape.setPosition(shape.getPosition().x, currentBottom - targetHeight);
    }

    velocityY += GRAVITY;
    shape.move(0, velocityY);

    if (shape.getPosition().y + shape.getSize().y >= groundLevel) {
        shape.setPosition(shape.getPosition().x, groundLevel - shape.getSize().y);
        velocityY = 0.0f;
        isGrounded = true;
    }
}

void Dino::takeHit() {
    if (state == State::Mort) return;
    if (invincibilityTimer > 0) return;

    hitCount++;
    if (hitCount == 1) {
        state = State::Blesse;
    } else if (hitCount >= MAX_HITS) {
        state = State::Mort;
        shape.setFillColor(sf::Color(128, 128, 128));
    }

    invincibilityTimer = INVINCIBILITY_FRAMES;
}

void Dino::reset() {
    state = State::OK;
    hitCount = 0;
    invincibilityTimer = 0.0f;
    velocityY = 0.0f;
    isGrounded = true;
    isCrouching = false;
    shape.setPosition(100, 250);
    shape.setSize({WIDTH, NORMAL_HEIGHT});
    shape.setFillColor(sf::Color::Black);
}

void Dino::draw(sf::RenderWindow& window) const {
    if (shape.getFillColor().a > 0 || state == State::OK) {
        window.draw(shape);
    }
}

sf::FloatRect Dino::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

int Dino::getLives() const {
    return MAX_HITS - hitCount;
}

Dino::State Dino::getState() const {
    return state;
}