#include "Bloc.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"
#include <iostream>
#include <cmath>

// ======================== Classe Bloc ========================
// Constructeur
Bloc::Bloc(const std::string& texturePath, BlocType type) : type(type) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement texture : " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(64.0f / texture.getSize().x, 64.0f / texture.getSize().y);
}

// ======================== BlocSol ========================
// Constructeur
BlocSol::BlocSol() : Bloc("../img/block.png", BlocType::SOL) {}

// ======================== BlocMystere ========================
// Constructeur
BlocMystere::BlocMystere() : Bloc("../img/mystery_box.png", BlocType::MYSTERE) {
    startPosition = sprite.getPosition();

    if (!textureFoncee.loadFromFile("../img/mystery_box_dark.png")) {
        std::cerr << "Erreur chargement texture foncée!" << std::endl;
    }
}

// Si le bloc mystère est touché
void BlocMystere::onHit() {
    if (!animating && sprite.getTexture() != &textureFoncee) {
        animating = true;
        animationTime = 0.0f;
        startPosition = sprite.getPosition();

        // Crée la pièce seulement si texture originale
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

// ======================== Tuyau ========================
// Constructeur
Tuyau::Tuyau(TypeES type) : Bloc("../img/pipe.png", BlocType::TUYAU), type(type) {}

// Vérifie si le joueur est au-dessus du tuyau
bool Tuyau::isPlayerOnTop(const Player& player) const {
    sf::FloatRect playerBounds = player.getGlobalBounds();
    sf::FloatRect tuyauBounds = getGlobalBounds();

    float playerCenterX = playerBounds.left + playerBounds.width / 2.0f;
    float tuyauCenterX = tuyauBounds.left + tuyauBounds.width / 2.0f;

    float maxOffset = tuyauBounds.width * 0.5f; // 30% du tuyau max sur les côtés

    bool horizontalOk = std::abs(playerCenterX - tuyauCenterX) < maxOffset;
    bool verticalOk = (playerBounds.top + playerBounds.height) <= tuyauBounds.top + 5.0f &&
                      (playerBounds.top + playerBounds.height) >= tuyauBounds.top - 5.0f;

    return horizontalOk && verticalOk;
}
