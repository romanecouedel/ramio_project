// Player.cpp
#include "Player.h"
#include <iostream>

Player::Player()
{
    if (!texture.loadFromFile("../img/mario.png"))
    {
        std::cerr << "Erreur chargement Mario" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    sprite.setPosition(100, 100);
}

void Player::handleInput()
{
    velocity.x = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        velocity.x = -speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        velocity.x = speed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canJump)
        jump();
}

void Player::jump()
{ 
    velocity.y = jumpForce;
    canJump = false;
}

void Player::update(float deltaTime, const Level &level)
{
    velocity.y += gravity * deltaTime;

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
    return bounds; //coucou c moi
}

//Wesh ça dit !! <3
