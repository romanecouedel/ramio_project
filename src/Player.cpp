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


void Player::respawn() {
    sprite.setPosition(100, 100); // Position de base, à ajuster
    velocity = {0, 0}; // Réinitialiser la vitesse
}
void Player::draw(sf::RenderWindow& window) const {
    if (visible) {
        window.draw(sprite);
    }
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

void Player::update(float deltaTime)
{
    handleInput();
}

sf::FloatRect Player::getHitbox() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds; 
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}
void Player::setOpacity(sf::Uint8 alpha) {
    sf::Color color = sprite.getColor();
    color.a = alpha;
    sprite.setColor(color);
}

void Player::jump() {
    velocity.y = jumpForce;
    canJump = false;
    currentAnimation = faceRight ? &animationJumpRight : &animationJumpLeft;
    audioManager.playYahooSound();
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

void Mario::respawn() {
    sprite.setPosition(100, 100);
    velocity = {0, 0};
    isDead = false;
}

void Mario::update(float deltaTime, const Level& level) {
    Player::update(deltaTime, level);
}



//==========================LUIGI==========================//

Luigi::Luigi(bool aiMode, Level* lvl , const Mario* mario) : isAI(aiMode), level(lvl), mario(mario) {
    if (!texture.loadFromFile("../img/sprite_luigi.png")) std::cerr << "Erreur chargement Luigi" << std::endl;
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

void Luigi::marcher_normal() {
    sf::FloatRect hitbox = getHitbox();
             
            // Vérifie si Luigi est sur le point de dépasser Mario
            if (mario->faceRight) {
                hitbox.left += 0.5*speed;// Simulation de la position future à droite
                if (sprite.getPosition().x > mario->getPosition().x -100) {
                    velocity.x = 0; // Arrête Luigi pour ne pas dépasser Mario
                } else {
                    velocity.x = speed;
                    faceRight = true;
                    currentAnimation = &animationWalkRight;
                }
                if (level->isColliding(hitbox) && canJump) {
                    jump();
                    velocity.x = speed; // Continue à avancer vers la droite après le saut
                    faceRight = true;
                    currentAnimation = &animationJumpRight;
                }
            } else {
                hitbox.left -= 0.5*speed;// Simulation de la position future à droite
                if (sprite.getPosition().x < mario->getPosition().x +100) {
                    velocity.x = 0; // Arrête Luigi pour ne pas dépasser Mario
                } else {
                    velocity.x = -speed;
                    faceRight = false;
                    currentAnimation = &animationWalkLeft;
                }
                if (level->isColliding(hitbox) && canJump) {
                    jump();
                    velocity.x = -speed; // Continue à avancer vers la gauche après le saut
                    faceRight = false;
                    currentAnimation = &animationJumpLeft;
                }
            }

}


void Luigi::handleInput() {
if (isAI) {
    BlocMystere* blocProche = level->getBlocMystereProche(sprite.getPosition());
    if (blocProche != nullptr) {
        sf::Vector2f position = blocProche->getPosition();
        if (position.x < sprite.getPosition().x && position.y>sprite.getPosition().y-200.0f && !blocProche->estTouche) {
            // Chemin pour aller jusqu'à la boîte
            velocity.x = -speed;
            faceRight = false;
            currentAnimation = &animationWalkLeft;
            // Vérifie si Luigi a atteint le bloc mystère
            if (std::abs(sprite.getPosition().x - position.x) < 20.0f && sprite.getPosition().y>position.y && onGround){                // Je suis en dessous de la boîte
                jump();

            }
        }
        else{
            marcher_normal();
        }
            
    } else{
            marcher_normal();
    } 
}
else {
        // Contrôle manuel
        
        velocity.x = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) { velocity.x = -speed; faceRight = false; currentAnimation = &animationWalkLeft; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { velocity.x = speed; faceRight = true; currentAnimation = &animationWalkRight; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && canJump) { jump(); }
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