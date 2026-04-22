#include "Dino.hpp"
#include <iostream>

Dino::Dino() {
    width = 80.0f;  // Scaled down from 209 to fit screen better
    standingHeight = 80.0f;
    crouchHeight = 55.0f;  // Adjusted crouch height
    
    position = sf::Vector2f(100, GROUND_Y - standingHeight);
    velocityY = 0;
    isGrounded = true;
    isJumping = false;
    isCrouching = false;
    
    // Health system initialization
    health = 3;
    state = DinoState::OK;
    invincibilityTimer = 0;
    invincibilityDuration = 1.5f;
    isInvincible = false;
    
    // Animation initialization
    currentFrame = 0;
    animationTimer = 0;
    animationSpeed = 0.15f;
    currentAnimation = AnimationState::IDLE;
    
    loadSpriteSheet();
}

void Dino::loadSpriteSheet() {
    // Load the sprite sheet
    if (!spriteSheet.loadFromFile("resources/sprites/DinoSpriteSheet.png")) {
        std::cerr << "Failed to load dino sprite sheet! Creating fallback..." << std::endl;
        createFallbackSprite();
        return;
    }
    
    // Sprite sheet dimensions
    int frameWidth = 209;   // Original width
    int frameHeight = 198;  // Original height
    
    // Store original scale factors
    float scaleX = width / frameWidth;
    float scaleY = standingHeight / frameHeight;
    
    // Frame indices (0-based, arranged horizontally)
    // Index 0: Idle
    // Index 1: Run frame 1
    // Index 2: Run frame 2  
    // Index 3: Jump
    // Index 4: Crouch
    // Index 5: Death
    
    // Idle frame (index 0)
    idleFrame = sf::IntRect(0 * frameWidth, 0, frameWidth, frameHeight);
    
    // Running frames (indices 1 and 2)
    runFrames.push_back(sf::IntRect(1 * frameWidth, 0, frameWidth, frameHeight));
    runFrames.push_back(sf::IntRect(2 * frameWidth, 0, frameWidth, frameHeight));
    
    // Jump frame (index 3)
    jumpFrame = sf::IntRect(3 * frameWidth, 0, frameWidth, frameHeight);
    
    // Crouch frame (index 4) - Use the FULL frame but we'll scale it differently
    // Instead of cropping, use the whole frame and rely on sprite scaling
    crouchFullFrame = sf::IntRect(4 * frameWidth, 0, frameWidth, frameHeight);
    
    // Death frame (index 5)
    deadFrame = sf::IntRect(5 * frameWidth, 0, frameWidth, frameHeight);
    
    // Hurt frame (use idle frame with red tint)
    hurtFrame = idleFrame;
    
    // Set initial scale
    sprite.setScale(scaleX, scaleY);
    sprite.setTexture(spriteSheet);
    sprite.setPosition(position);
    
    // Set initial frame
    setAnimation(AnimationState::IDLE);
    
    std::cout << "Sprite sheet loaded successfully!" << std::endl;
    std::cout << "Frame size: " << frameWidth << "x" << frameHeight << std::endl;
    std::cout << "Scale: " << scaleX << "x" << scaleY << std::endl;
}

void Dino::createFallbackSprite() {
    // Create a fallback colored dino if sprite sheet doesn't exist
    sf::Image image;
    image.create(80, 80, sf::Color::Transparent);
    
    // Draw a simple dino shape
    for (int x = 0; x < 80; x++) {
        for (int y = 0; y < 80; y++) {
            if (x > 10 && x < 70 && y > 20 && y < 70) {
                image.setPixel(x, y, sf::Color::Black);
            }
            if (x > 60 && x < 76 && y > 10 && y < 40) {
                image.setPixel(x, y, sf::Color::Black);
            }
            if (x > 4 && x < 20 && y > 50 && y < 64) {
                image.setPixel(x, y, sf::Color::Black);
            }
            if (x > 20 && x < 36 && y > 64 && y < 76) {
                image.setPixel(x, y, sf::Color::Black);
            }
            if (x > 44 && x < 60 && y > 64 && y < 76) {
                image.setPixel(x, y, sf::Color::Black);
            }
            if (x == 68 && y == 24) {
                image.setPixel(x, y, sf::Color::White);
            }
        }
    }
    
    sf::Texture fallbackTexture;
    fallbackTexture.loadFromImage(image);
    spriteSheet = fallbackTexture;
    
    // Set up basic frames for fallback
    idleFrame = sf::IntRect(0, 0, 80, 80);
    runFrames.push_back(sf::IntRect(0, 0, 80, 80));
    runFrames.push_back(sf::IntRect(0, 0, 80, 80));
    jumpFrame = sf::IntRect(0, 0, 80, 80);
    crouchFullFrame = sf::IntRect(0, 0, 80, 80);
    deadFrame = sf::IntRect(0, 0, 80, 80);
    hurtFrame = idleFrame;
    
    sprite.setTexture(spriteSheet);
    sprite.setPosition(position);
    setAnimation(AnimationState::IDLE);
    
    std::cout << "Fallback sprite created" << std::endl;
}

void Dino::setAnimation(AnimationState newAnimation) {
    if (currentAnimation == newAnimation) return;
    
    currentAnimation = newAnimation;
    currentFrame = 0;
    animationTimer = 0;
    
    // Reset scale to normal first
    float frameWidth = 209;
    float frameHeight = 198;
    float scaleX = width / frameWidth;
    float scaleY = standingHeight / frameHeight;
    
    // Set the frame and scale based on animation
    switch (currentAnimation) {
        case AnimationState::IDLE:
            sprite.setTextureRect(idleFrame);
            sprite.setScale(scaleX, scaleY);
            break;
        case AnimationState::RUNNING:
            if (!runFrames.empty()) {
                sprite.setTextureRect(runFrames[0]);
                sprite.setScale(scaleX, scaleY);
            }
            break;
        case AnimationState::JUMPING:
            sprite.setTextureRect(jumpFrame);
            sprite.setScale(scaleX, scaleY);
            break;
        case AnimationState::CROUCHING:
            sprite.setTextureRect(crouchFullFrame);
            // Scale differently for crouch - squash vertically
            sprite.setScale(scaleX, scaleY * 0.65f);
            break;
        case AnimationState::HURT:
            sprite.setTextureRect(hurtFrame);
            sprite.setScale(scaleX, scaleY);
            break;
        case AnimationState::DEAD:
            sprite.setTextureRect(deadFrame);
            sprite.setScale(scaleX, scaleY);
            break;
    }
}

void Dino::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;
    
    if (animationTimer >= animationSpeed) {
        animationTimer = 0;
        
        switch (currentAnimation) {
            case AnimationState::RUNNING:
                if (!runFrames.empty()) {
                    currentFrame = (currentFrame + 1) % runFrames.size();
                    sprite.setTextureRect(runFrames[currentFrame]);
                }
                break;
                
            default:
                // Other animations don't cycle
                break;
        }
    }
}

void Dino::update(float deltaTime) {
    // Update invincibility timer
    if (isInvincible) {
        invincibilityTimer += deltaTime;
        if (invincibilityTimer >= invincibilityDuration) {
            isInvincible = false;
            invincibilityTimer = 0;
        }
    }
    
    // Update animation based on state
    if (state == DinoState::DEAD) {
        setAnimation(AnimationState::DEAD);
    } else if (isInvincible) {
        setAnimation(AnimationState::HURT);
    } else if (isCrouching) {
        setAnimation(AnimationState::CROUCHING);
    } else if (!isGrounded) {
        setAnimation(AnimationState::JUMPING);
    } else if (isGrounded && !isCrouching && !isJumping) {
        setAnimation(AnimationState::RUNNING);
    } else {
        setAnimation(AnimationState::IDLE);
    }
    
    updateAnimation(deltaTime);
    
    // Only apply gravity if not dead
    if (state != DinoState::DEAD) {
        // Apply gravity
        velocityY += GRAVITY * deltaTime;
        position.y += velocityY * deltaTime;
        
        // Ground collision - use current height based on crouching state
        float currentHeight = isCrouching ? crouchHeight : standingHeight;
        if (position.y >= GROUND_Y - currentHeight) {
            position.y = GROUND_Y - currentHeight;
            velocityY = 0;
            isGrounded = true;
            isJumping = false;
        } else {
            isGrounded = false;
        }
    }
    
    sprite.setPosition(position);
}

void Dino::jump() {
    if (isGrounded && !isCrouching && state != DinoState::DEAD) {
        velocityY = JUMP_FORCE;
        isGrounded = false;
        isJumping = true;
    }
}

void Dino::crouch() {
    if (isGrounded && !isJumping && state != DinoState::DEAD && !isCrouching) {
        isCrouching = true;
        position.y = GROUND_Y - crouchHeight;
        sprite.setPosition(position);
    }
}

void Dino::stand() {
    if (isCrouching && state != DinoState::DEAD) {
        isCrouching = false;
        position.y = GROUND_Y - standingHeight;
        sprite.setPosition(position);
        // Reset scale will happen in setAnimation when animation changes
    }
}

void Dino::takeDamage() {
    if (!isInvincible && state != DinoState::DEAD) {
        health--;
        
        if (health <= 0) {
            health = 0;
            state = DinoState::DEAD;
            setAnimation(AnimationState::DEAD);
        } else if (health == 1) {
            state = DinoState::HURT;
            isInvincible = true;
        } else if (health == 2) {
            state = DinoState::HURT;
            isInvincible = true;
        } else {
            isInvincible = true;
        }
        
        // Force stand up when hit
        if (isCrouching) {
            stand();
        }
        
        std::cout << "Dino took damage! Health: " << health << std::endl;
    }
}

void Dino::heal() {
    if (health < 3 && state != DinoState::DEAD) {
        health++;
        if (health == 3) {
            state = DinoState::OK;
        }
        std::cout << "Dino healed! Health: " << health << std::endl;
    }
}

void Dino::draw(sf::RenderWindow& window) {
    // Handle invincibility blinking
    if (isInvincible) {
        if (static_cast<int>(invincibilityTimer * 10) % 2 == 0) {
            sprite.setColor(sf::Color(255, 255, 255, 200));
        } else {
            sprite.setColor(sf::Color(255, 100, 100, 200));
        }
    } else if (state == DinoState::HURT) {
        sprite.setColor(sf::Color(255, 150, 150));
    } else if (state == DinoState::DEAD) {
        sprite.setColor(sf::Color(150, 150, 150));
        sprite.setRotation(90);
    } else {
        sprite.setColor(sf::Color::White);
        sprite.setRotation(0);
    }

    sf::RectangleShape hitbox(sf::Vector2f(getBounds().width, getBounds().height));
    hitbox.setPosition(getBounds().left, getBounds().top);
    hitbox.setFillColor(sf::Color(255, 0, 0, 100));
    window.draw(hitbox);
    
    window.draw(sprite);
    
    // Reset sprite color for next frame
    if (!isInvincible && state != DinoState::HURT) {
        sprite.setColor(sf::Color::White);
    }
}

sf::FloatRect Dino::getBounds() const {
    // Get the actual sprite bounds
    sf::FloatRect bounds = sprite.getGlobalBounds();
    
    // Make hitbox smaller for fairer gameplay
    float hitboxWidth;
    float hitboxHeight;
    float hitboxX;
    float hitboxY;
    
    if (isCrouching) {
        // Crouching hitbox - smaller and lower
        hitboxWidth = bounds.width * 0.55f;   // 65% of sprite width
        hitboxHeight = bounds.height * 0.5f;  // 70% of sprite height
        hitboxX = bounds.left + (bounds.width - hitboxWidth) / 2;
        hitboxY = bounds.top + (bounds.height - hitboxHeight);
    } else if (!isGrounded) {
        // Jumping hitbox - slightly smaller
        hitboxWidth = bounds.width * 0.55f;    // 60% of sprite width
        hitboxHeight = bounds.height * 0.7f; // 75% of sprite height
        hitboxX = bounds.left + (bounds.width - hitboxWidth) / 2;
        hitboxY = bounds.top + (bounds.height - hitboxHeight) * 0.3f;
    } else {
        // Standing/Running hitbox
        hitboxWidth = bounds.width * 0.55f;   // 55% of sprite width
        hitboxHeight = bounds.height * 0.7f;  // 70% of sprite height
        hitboxX = bounds.left + (bounds.width - hitboxWidth) / 2;
        hitboxY = bounds.top + (bounds.height - hitboxHeight);
    }
    
    return sf::FloatRect(hitboxX, hitboxY, hitboxWidth, hitboxHeight);
}

void Dino::reset() {
    position = sf::Vector2f(100, GROUND_Y - standingHeight);
    velocityY = 0;
    isGrounded = true;
    isJumping = false;
    isCrouching = false;
    health = 3;
    state = DinoState::OK;
    isInvincible = false;
    invincibilityTimer = 0;
    sprite.setPosition(position);
    sprite.setColor(sf::Color::White);
    sprite.setRotation(0);
    currentAnimation = AnimationState::IDLE;
    setAnimation(AnimationState::IDLE);
}

bool Dino::isCrouchingState() const {
    return isCrouching;
}

bool Dino::isJumpingState() const {
    return isJumping;
}

DinoState Dino::getState() const {
    return state;
}

int Dino::getHealth() const {
    return health;
}

bool Dino::isInvincibleState() const {
    return isInvincible;
}