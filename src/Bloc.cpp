// Bloc.cpp
#include "Bloc.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"
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

// ======================== Tuyau ========================
Tuyau::Tuyau(Type type) : Bloc("../img/pipe.png"), type(type) {}

Tuyau::Type Tuyau::getType() const {
    return type;
}

Tuyau* Tuyau::getSortieAssociee(const std::vector<std::unique_ptr<Bloc>>& blocs) const {
    Tuyau* meilleureSortie = nullptr;
    float meilleureDistance = std::numeric_limits<float>::max();

    for (const auto& bloc : blocs) {
        Tuyau* sortie = dynamic_cast<Tuyau*>(bloc.get());
        if (sortie && sortie->getType() == Tuyau::Type::SORTIE) {
            float distance = std::abs(sortie->getPosition().x - this->getPosition().x);
            if (distance < meilleureDistance) {
                meilleureDistance = distance;
                meilleureSortie = sortie;
            }
        }
    }
    return meilleureSortie;
}



bool Tuyau::isPlayerOnTop(const Player& player) const {
    sf::FloatRect playerBounds = player.getHitbox();
    sf::FloatRect tuyauBounds = getGlobalBounds();

    float playerCenterX = playerBounds.left + playerBounds.width / 2.0f;
    float tuyauCenterX = tuyauBounds.left + tuyauBounds.width / 2.0f;

    float maxOffset = tuyauBounds.width * 0.5f; // 50% du tuyau max sur les côtés

    bool horizontalOk = std::abs(playerCenterX - tuyauCenterX) < maxOffset;
    bool verticalOk = (playerBounds.top + playerBounds.height) <= tuyauBounds.top + 5.0f &&
                      (playerBounds.top + playerBounds.height) >= tuyauBounds.top - 5.0f;

    return horizontalOk && verticalOk;
}
