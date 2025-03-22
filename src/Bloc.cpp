#include "Bloc.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"
#include <iostream>
#include <cmath>
#include <cmath>

// ======================== Initialisation des Textures Globales ========================
sf::Texture Bloc::textureBlocSol;
sf::Texture Bloc::textureBlocMystere;
sf::Texture Bloc::textureTuyau;

// ======================== BlocSol ========================
BlocSol::BlocSol() {
    if (textureBlocSol.getSize().x == 0) { // Charge la texture une seule fois
        if (!textureBlocSol.loadFromFile("../img/block.png")) {
            std::cerr << "Erreur chargement texture BlocSol" << std::endl;
        }
    }
    sprite.setTexture(textureBlocSol);
    sprite.setScale(64.0f / textureBlocSol.getSize().x, 64.0f / textureBlocSol.getSize().y);
}

// ======================== BlocMystere ========================
BlocMystere::BlocMystere() {
    if (textureBlocMystere.getSize().x == 0) { // Charge la texture une seule fois
        if (!textureBlocMystere.loadFromFile("../img/mystery_box.png")) {
            std::cerr << "Erreur chargement texture BlocMystere" << std::endl;
        }
    }
    sprite.setTexture(textureBlocMystere);
    sprite.setScale(64.0f / textureBlocMystere.getSize().x, 64.0f / textureBlocMystere.getSize().y);
    
    startPosition = sprite.getPosition();

    if (!textureFoncee.loadFromFile("../img/mystery_box_dark.png")) {
        std::cerr << "Erreur chargement texture foncée BlocMystere" << std::endl;
    }
}

void BlocMystere::onHit() {
    if (!animating && sprite.getTexture() != &textureFoncee) {
        animating = true;
        animationTime = 0.0f;
        startPosition = sprite.getPosition();

        // Crée la pièce seulement si la texture est encore la boîte mystère
        piece = std::make_unique<Piece>(startPosition.x, startPosition.y - 64.0f);

        // Change la texture du bloc
        changerTexture();
    }
}

// changement texture lorsque la pièce est déjà récupérée
void BlocMystere::changerTexture() {
    sprite.setTexture(textureFoncee);
}

// Met à jour l'animation du bloc mystère
void BlocMystere::update(float deltaTime, sf::RenderWindow& window) {
    if (animating) {
        animationTime += deltaTime;
        float offset = -animationHeight * std::sin((animationTime / animationDuration) * 3.14159f);
        sprite.setPosition(startPosition.x, startPosition.y + offset);

        if (animationTime >= animationDuration) {
            sprite.setPosition(startPosition);
            animating = false;
            animationTime = 0.f;
        }
    }
}

bool BlocMystere::isAnimating() const {
    return animating;
}

// ======================== Tuyau ========================
Tuyau::Tuyau(Type type) : type(type) {
    if (textureTuyau.getSize().x == 0) { // Charge la texture une seule fois
        if (!textureTuyau.loadFromFile("../img/pipe.png")) {
            std::cerr << "Erreur chargement texture Tuyau" << std::endl;
        }
    }
    sprite.setTexture(textureTuyau);
    sprite.setScale(64.0f / textureTuyau.getSize().x, 64.0f / textureTuyau.getSize().y);
}

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
    sf::FloatRect playerBounds = player.getGlobalBounds();
    sf::FloatRect tuyauBounds = getGlobalBounds();

    float playerCenterX = playerBounds.left + playerBounds.width / 2.0f;
    float tuyauCenterX = tuyauBounds.left + tuyauBounds.width / 2.0f;

    float maxOffset = tuyauBounds.width * 0.5f; // 50% du tuyau max sur les côtés

    bool horizontalOk = std::abs(playerCenterX - tuyauCenterX) < maxOffset;
    bool verticalOk = (playerBounds.top + playerBounds.height) <= tuyauBounds.top + 5.0f &&
                      (playerBounds.top + playerBounds.height) >= tuyauBounds.top - 5.0f;

    return horizontalOk && verticalOk;
}
