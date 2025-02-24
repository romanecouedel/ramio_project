#include "Enemy.h"
#include <iostream>

Koopa::Koopa
() {

    if (!texture.loadFromFile("../img/koopa.png"))
    {
        std::cerr << "Erreur chargement Koopa" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    sprite.setPosition(500, 100);

}

void Koopa::move_ia(){
    
}


void Enemy::update_collision(float deltaTime, const Level &level)
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

void Koopa::update(float deltaTime) {
    if (isShell) {
        shellTimer += deltaTime;
        if (shellTimer >= shellDuration) {
            isShell = false;
            shellTimer = 0.f;
        }
    }
    else {
        if (direction) {
            velocity.x = speed;
        }
        else {
            velocity.x = -speed;
        }
        distance += speed * deltaTime;
        if (distance >= maxDistance) {
            direction = !direction;
            distance = 0.f;
        }
    }
    sprite.move(velocity * deltaTime);
}

void Enemy::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}
