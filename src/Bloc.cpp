// Bloc.cpp
#include "Bloc.h"
#include "Entity.h"
#include "Level.h"
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
    sprite.setScale(64.0f / texture.getSize().x, 64.0f / texture.getSize().y);
}

// ======================== BlocSol ========================
BlocSol::BlocSol() : Bloc("../img/block.png") {}

// ======================== BlocMystere ========================
BlocMystere::BlocMystere() : Bloc("../img/mystery_box.png")
{
    startPosition = sprite.getPosition();

    if (!textureFoncee.loadFromFile("../img/mystery_box_dark.png"))
    {
        std::cerr << "Erreur chargement texture foncée!" << std::endl;
    }
}

void BlocMystere::changerTexture()
{
    sprite.setTexture(textureFoncee);
}

void BlocMystere::onHit()
{
    if (!animating && sprite.getTexture() != &textureFoncee)
    {
        animating = true;
        animationTime = 0.0f;
        startPosition = sprite.getPosition();

        // Crée la pièce seulement si texture originale
        piece = std::make_unique<Piece>(startPosition.x, startPosition.y - 64.0f);

        // Change la texture du bloc
        changerTexture();

    }
}

void BlocMystere::update(float deltaTime, sf::RenderWindow &window)
{
    if (animating)
    {
        animationTime += deltaTime;

        float offset = -animationHeight * std::sin((animationTime / animationDuration) * 3.14159f);
        sprite.setPosition(startPosition.x, startPosition.y + offset);

        if (animationTime >= animationDuration)
        {
            sprite.setPosition(startPosition);
            animating = false;
            animationTime = 0.f;
        }
    }

}

bool BlocMystere::isAnimating() const
{
    return animating;
}

