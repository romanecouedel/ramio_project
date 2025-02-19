// Player.cpp
#include "Player.h"
#include "Animation.h"
#include <iostream>

Player::Player()
{
    if (!texture.loadFromFile("../img/sprite_mario.png"))
    {
        std::cerr << "Erreur chargement Mario" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.40f, 0.40f);
    sprite.setPosition(100, 100);

    // init des animations
    animationWalkRight = Animation(&texture, {1, 1}, 0.1f);
    animationWalkLeft = Animation(&texture, {1, 2}, 0.1f);
    animationJumpLeft = Animation(&texture, {4, 1}, 0.1f);
    animationJumpRight = Animation(&texture, {2, 1}, 0.1f);

    animationIdleRight = Animation(&texture,{1,1} ,0.f);
    animationIdleLeft = Animation(&texture,{1,2},0.f);

    currentAnimation = &animationWalkRight;
}

void Player::handleInput()
{
    velocity.x = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        velocity.x = -speed;
        faceRight = false;
        currentAnimation = &animationWalkLeft;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        velocity.x = speed;
        faceRight = true;
        currentAnimation = &animationWalkRight;
        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canJump)
    {
        jump();
        currentAnimation = faceRight ? &animationJumpRight : &animationJumpLeft;
    }
}

void Player::jump()
{
    velocity.y = jumpForce;
    canJump = false;
    currentAnimation = faceRight ? &animationJumpRight : &animationJumpLeft;
}

void Player::update(float deltaTime, const Level &level)
{
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

void Player::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

sf::FloatRect Player::getHitbox() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds; // coucou c moi
}
