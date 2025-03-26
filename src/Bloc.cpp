#include "Bloc.h"
#include "Entity.h"
#include "Level.h"
#include "Player.h"
#include <iostream>
#include <cmath>

// ======================== Initialisation des Textures Globales ========================
sf::Texture Bloc::textureBlocSol;
sf::Texture Bloc::textureBlocMystere;
sf::Texture Bloc::textureTuyau;
sf::Texture Eau::textureEau;

// ======================== BlocSol ========================
/**
 * @brief Constructeur de BlocSol, charge la texture si ce n'est pas encore fait.
 */
BlocSol::BlocSol() {
    if (textureBlocSol.getSize().x == 0) {
        if (!textureBlocSol.loadFromFile("../img/block.png")) {
            std::cerr << "Erreur chargement texture BlocSol" << std::endl;
        }
    }
    sprite.setTexture(textureBlocSol);
    sprite.setScale(64.0f / textureBlocSol.getSize().x, 64.0f / textureBlocSol.getSize().y);
}

// ======================== BlocMystere ========================
/**
 * @brief Constructeur de BlocMystere, charge la texture si ce n'est pas encore fait.
 */
BlocMystere::BlocMystere() {
    if (textureBlocMystere.getSize().x == 0) {
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

/**
 * @brief Action lorsque le bloc mystère est frappé.
 */
void BlocMystere::onHit() {
    if (!animating && sprite.getTexture() != &textureFoncee) {//si l'animatione est pas deja en cours et que la texture a pas changé
        animating = true;
        animationTime = 0.0f;
        startPosition = sprite.getPosition();

        // Crée la pièce
        piece = std::make_unique<Piece>(startPosition.x, startPosition.y - 64.0f);

        // Change la texture du bloc
        changerTexture();
    }
}

/**
 * @brief Change la texture du bloc mystère après activation.
 */
void BlocMystere::changerTexture() {
    sprite.setTexture(textureFoncee);
}

/**
 * @brief Met à jour l'animation du bloc mystère.
 * @param deltaTime Temps écoulé depuis la dernière mise à jour.
 * @param window Fenêtre SFML.
 */
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

/**
 * @brief Vérifie si l'animation du bloc mystère est en cours.
 * @return True si en animation, False sinon.
 */
bool BlocMystere::isAnimating() const {
    return animating;
}

// ======================== Tuyau ========================
/**
 * @brief Constructeur de Tuyau, charge la texture si ce n'est pas encore fait.
 * @param type Type du tuyau (entrée ou sortie).
 */
Tuyau::Tuyau(Type type) : type(type) {
    if (textureTuyau.getSize().x == 0) {
        if (!textureTuyau.loadFromFile("../img/pipe.png")) {
            std::cerr << "Erreur chargement texture Tuyau" << std::endl;
        }
    }
    sprite.setTexture(textureTuyau);
    sprite.setScale(64.0f / textureTuyau.getSize().x, 64.0f / textureTuyau.getSize().y);
}

/**
 * @brief Retourne le type du tuyau (entrée ou sortie).
 * @return Type du tuyau.
 */
Tuyau::Type Tuyau::getType() const {
    return type;
}

/**
 * @brief Trouve la sortie associée au tuyau.
 * @param blocs Liste des blocs du niveau.
 * @return Pointeur vers le tuyau de sortie correspondant.
 */
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

/**
 * @brief Vérifie si le joueur est sur le tuyau.
 * @param player Référence vers le joueur.
 * @return True si le joueur est au-dessus du tuyau, False sinon.
 */
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

// ======================== Eau ========================
/**
 * @brief Constructeur de la classe Eau, charge la texture si ce n'est pas encore fait.
 */
Eau::Eau() {
    if (textureEau.getSize().x == 0) {
        if (!textureEau.loadFromFile("../img/eau.png")) {
              std::cerr << "Erreur chargement texture Eau" << std::endl;
        }
    }
    sprite.setTexture(textureEau);
    sprite.setScale(64.0f / textureEau.getSize().x, 64.0f / textureEau.getSize().y);
    sf::Color color = sprite.getColor(); 
    color.a = 150;  
    sprite.setColor(color); 
}



/**
 * @brief Vérifie si un joueur est dans l'eau.
 * @param player Le joueur à vérifier (Mario ou Luigi).
 * @return True si le joueur est dans l'eau, False sinon.
 */
bool Eau::isPlayerInWater(const sf::FloatRect& playerHitbox) const
{
    return playerHitbox.intersects(this->getGlobalBounds());  // Vérifier si la hitbox du joueur est dans  l'eau
}


/**
 * @brief Affiche l'eau sur l'écran.
 * @param window Fenêtre SFML.
 */
void Eau::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

/**
 * @brief Retourne les coordonnées du bloc d'eau.
 * @return Les coordonnées du bloc.
 */
sf::FloatRect Eau::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
