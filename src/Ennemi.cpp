#include "Ennemi.h"
#include <iostream>

sf::Texture Ennemi::textureEnnemi; // Texture statique

Ennemi::Ennemi()
{
    if (textureEnnemi.getSize().x == 0) // Charge la texture une seule fois
    {
        if (!textureEnnemi.loadFromFile("../img/goomba.png"))
        {
            std::cerr << "Erreur chargement texture Ennemi" << std::endl;
        }
    }
    sprite.setTexture(textureEnnemi);
    
    // Calcul automatique de la taille d'une frame
    frameWidth = textureEnnemi.getSize().x / 3; // 3 images sur la ligne
    frameHeight = textureEnnemi.getSize().y;
    
    // Appliquer la première image
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

    sprite.setScale(64.0f / frameWidth, 64.0f / frameHeight);

    velocity.x = -50.f; 
    velocity.y = 0.f;
    isSquashed = false;
    isAlive = true;
}

void Ennemi::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

void Ennemi::update(float deltaTime, Level &level)
{
    if (isSquashed)
    {
        squashTimer += deltaTime;
        if (squashTimer >= 1.0f) // Après 1 seconde, l'ennemi disparaît
        {
            isAlive = false;
        }
        return;
    }

    // Animation de marche (2 premières images)
    animationTimer += deltaTime;
    if (animationTimer > 0.2f)
    {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % 2; // Alterne entre 0 et 1
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight)); 
    }

    // Déplacement et gestion des collisions
    sf::Vector2f position = sprite.getPosition();
    sf::FloatRect hitbox = sprite.getGlobalBounds();
    sf::Vector2f leftPoint(position.x - 2.0f, position.y + hitbox.height / 2);
    sf::Vector2f rightPoint(position.x + hitbox.width + 2.0f, position.y + hitbox.height / 2);

    bool collisionLeft = level.isColliding(sf::FloatRect(leftPoint.x, leftPoint.y, 1.0f, 1.0f));
    bool collisionRight = level.isColliding(sf::FloatRect(rightPoint.x, rightPoint.y, 1.0f, 1.0f));

    if (collisionLeft && velocity.x < 0)
    {
        velocity.x = 50.f;
    }
    else if (collisionRight && velocity.x > 0)
    {
        velocity.x = -50.f;
    }

    sprite.move(velocity.x * deltaTime, 0);
}

void Ennemi::onPlayerCollision(bool fromAbove)
{
    if (fromAbove)
    {
        isSquashed = true;
        sprite.setTextureRect(sf::IntRect(2 * frameWidth, 0, frameWidth, frameHeight)); // Image écrasée
        velocity.x = 0;
    }
}

void Ennemi::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

sf::FloatRect Ennemi::getBounds() const
{
    return sprite.getGlobalBounds();
}
