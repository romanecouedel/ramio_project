// Player.cpp
#include "Player.h"
#include "Level.h"
#include "Animation.h"
#include "Audio.h"
#include <iostream>


extern AudioManager audioManager; 

Player::Player() {
    currentAnimation = &animationIdleRight;
}

void Player::update(float deltaTime, const Level &level)
{
    if (sprite.getPosition().y > level.getHeight() * 64 + 100) {//pour un peu de marge (64 pour la conversione n pixel)
        isDead = true;
    }
    velocity.y += gravity * deltaTime;
    if (velocity.y > 0 && onGround)
    { 
        isJumping = false;
    }
    if (velocity.x==0 && onGround)
    { 
        currentAnimation = faceRight ? &animationIdleRight : &animationIdleLeft;
    }
    currentAnimation->update(deltaTime, faceRight, isJumping);
    sprite.setTextureRect(currentAnimation->getCurrentFrame());

    if (velocity.y < 0)
    { 
        isJumping = true;
    }
    else if (velocity.y == 0)
    { 
        isJumping = false;
    }
    
    // Collisions horizontales
    sf::FloatRect hitbox = getHitbox();
    hitbox.left += velocity.x * deltaTime;
    if (!level.isColliding(hitbox))
    {
        sprite.move(velocity.x * deltaTime, 0);
    }
    else
    {
        velocity.x = 0;
    }

    // Collisions verticales
    hitbox = getHitbox();
    hitbox.top += velocity.y * deltaTime;

    if (!level.isColliding(hitbox))
    {
        sprite.move(0, velocity.y * deltaTime);
        onGround = false;
    }
    else
    {
        // Collision par le haut (plafond)
        if (velocity.y < 0)
        {
            while (level.isColliding(getHitbox()))
            {
                sprite.move(0, 0.5f); // Décale légèrement vers le bas
            }
            velocity.y = 0;
        }
        // Collision par le bas (sol)
        else if (velocity.y > 0)
        {
            while (level.isColliding(getHitbox()))
            {
                sprite.move(0, -0.5f); // Décale légèrement vers le haut
            }
            velocity.y = 0;
            onGround = true;
            canJump = true;
            isJumping = false; 
        }
    }
}

void Player::jump() {
    velocity.y = jumpForce;
    canJump = false;
    currentAnimation = faceRight ? &animationJumpRight : &animationJumpLeft;
    audioManager.playYahooSound();
}

void Player::draw(sf::RenderWindow& window) const {
    if (visible) {
        window.draw(sprite);
    }
}


void Player::respawn() {
    sprite.setPosition(100, 100); // Position de base, à ajuster
    velocity = {0, 0}; // Réinitialiser la vitesse
}

void Player::update(float deltaTime)
{
    handleInput();
}

sf::FloatRect Player::getHitbox() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds; // coucou c moi
}
void Player::setOpacity(sf::Uint8 alpha) {
    sf::Color color = sprite.getColor();
    color.a = alpha;
    sprite.setColor(color);
}


//==========================Classe dérivée==========================//
//========Mario======/

Mario::Mario() {
    if (!texture.loadFromFile("../img/sprite_mario.png")) std::cerr << "Erreur chargement Mario" << std::endl;
    sprite.setTexture(texture);
    sprite.setScale(0.40f, 0.40f);
    sprite.setPosition(100, 100);
    animationWalkRight = Animation(&texture, {1, 1}, 0.1f);
    animationWalkLeft = Animation(&texture, {1, 2}, 0.1f);
    animationJumpLeft = Animation(&texture, {4, 1}, 0.1f);
    animationJumpRight = Animation(&texture, {2, 1}, 0.1f);
    animationIdleRight = Animation(&texture,{1,1},0.f);
    animationIdleLeft = Animation(&texture,{1,2},0.f);
    currentAnimation = &animationIdleRight;
}

void Mario::handleInput() {
    velocity.x = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { velocity.x = -speed; faceRight = false; currentAnimation = &animationWalkLeft; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { velocity.x = speed; faceRight = true; currentAnimation = &animationWalkRight; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canJump) jump();
}

void Mario::update(float deltaTime, const Level& level) {
    Player::update(deltaTime, level);
}

void Mario::respawn() {
    sprite.setPosition(100, 100);
    velocity = {0, 0};
    isDead = false;
}


//=======Luigi=======/
Luigi::Luigi() {
    if (!texture.loadFromFile("../img/sprite_luigi.png")) 
        std::cerr << "Erreur chargement Luigi" << std::endl;
    
    sprite.setTexture(texture);
    sprite.setScale(0.40f, 0.40f);
    sprite.setPosition(150, 100);
    animationWalkRight = Animation(&texture, {1, 1}, 0.1f);
    animationWalkLeft = Animation(&texture, {1, 2}, 0.1f);
    animationJumpLeft = Animation(&texture, {4, 1}, 0.1f);
    animationJumpRight = Animation(&texture, {2, 1}, 0.1f);
    animationIdleRight = Animation(&texture,{1,1},0.f);
    animationIdleLeft = Animation(&texture,{1,2},0.f);
    currentAnimation = &animationIdleRight;
}

void Luigi::handleInput() {
    velocity.x = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { velocity.x = -speed; faceRight = false; currentAnimation = &animationWalkLeft; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { velocity.x = speed; faceRight = true; currentAnimation = &animationWalkRight; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && canJump) { jump(); }
    
}

void Luigi::handleAI(float deltaTime, const Mario& mario, const Level& level) {
    static float timer = 0.0f;
    
    float marioX = mario.getPosition().x;
    float luigiX = getPosition().x;

    float distance = marioX - luigiX;
    if (std::abs(distance) > 10.0f) {
        velocity.x = (distance > 0) ? speed*0.8: -speed*0.8;
    } else {
        velocity.x = 0;
    }

    // Vérifier les obstacles devant Luigi
    sf::FloatRect hitbox = getHitbox();
    hitbox.left += (velocity.x > 0) ? 10.0f : -10.0f; //un chouille devant
    bool obstacleDevant = level.isColliding(hitbox);

    // Vérifier s'il y a un trou sous les pieds
    sf::FloatRect hitboxBas = getHitbox();
    hitboxBas.top += 5.0f; // Vérifie en dessous
    bool solSousLesPieds = level.isColliding(hitboxBas);

    // Si obstacle devant ou trou détecté, Luigi saute
    if ((obstacleDevant || !solSousLesPieds) && canJump) {
        jump();
    }

    // Mettre à jour l'animation
    if (velocity.x < 0) {
        faceRight = false;
        currentAnimation = &animationWalkLeft;
    } else if (velocity.x > 0) {
        faceRight = true;
        currentAnimation = &animationWalkRight;
    } else {
        currentAnimation = faceRight ? &animationIdleRight : &animationIdleLeft;
    }
}

void Luigi::update(float deltaTime, const Level& level) {
    Player::update(deltaTime, level);
}

void Luigi::respawn() {
    sprite.setPosition(150, 100); 
    velocity = {0, 0};
    isDead = false;
}
