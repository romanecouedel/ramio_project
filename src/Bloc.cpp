// Bloc.cpp
#include "Bloc.h"
#include "Entity.h"
#include <iostream>
#include <math.h>

// ======================== Classe Bloc ========================
Bloc::Bloc(const std::string &texturePath)
{
    if (!texture.loadFromFile(texturePath))
    {
        std::cerr << "Erreur chargement texture : " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(32.0f / texture.getSize().x, 32.0f / texture.getSize().y);
}

// ======================== BlocSol ========================
BlocSol::BlocSol() : Bloc("../img/block.png") {}

// ======================== BlocMystere ========================
BlocMystere::BlocMystere() : Bloc("../img/mystery_box.png")
{
    startPosition = sprite.getPosition();
}

void BlocMystere::update(float deltaTime, sf::RenderWindow &window)
{
    if (animating)
    {
        animationTime += deltaTime;

        // Mouvement vertical en sinus (aller-retour)
        float offset = -animationHeight * std::sin((animationTime / animationDuration) * 3.14159f);
        sprite.setPosition(startPosition.x, startPosition.y + offset);

        if (animationTime >= animationDuration)
        {
            sprite.setPosition(startPosition);
            animating = false;
            animationTime = 0.f;
        }
        // Si la pièce existe, mettre à jour la pièce
        if (piece)
        {
            piece->draw(window);
        }
    }
}

void BlocMystere::onHit()
{
    if (!animating)
    {
        animating = true;
        animationTime = 0.0f;
        startPosition = sprite.getPosition();

        // Créer la pièce au-dessus du bloc mystère
        piece = std::make_unique<Piece>(startPosition.x, startPosition.y - 35.0f);
        
    }
}

bool BlocMystere::isAnimating() const
{
    return animating;
}
