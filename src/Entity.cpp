#include "Entity.h"

/**
 * @class Entity
 * @brief Classe de base pour toutes les entités du jeu.
 */
Entity::Entity(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement texture : " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
}

/**
 * @brief Retourne le rectangle englobant de l'entité.
 * @return Rectangle englobant de l'entité.
 */
sf::FloatRect Entity::getGlobalBounds() const {
    return sprite.getGlobalBounds(); // utilisé pour faire les collisions entre entités
}

/**
 * @class ObjetInteractif
 * @brief Classe représentant un objet interactif dans le jeu.
 */
ObjetInteractif::ObjetInteractif(const std::string& texturePath) : Entity(texturePath) {
    sprite.setScale(0.5f, 0.5f);
}

/**
 * @class Drapeau
 * @brief Classe représentant le drapeau de fin de niveau.
 */
Drapeau::Drapeau() : ObjetInteractif("../img/flag.png") {
    sprite.setScale(0.25f, 0.25f);
}

/**
 * @class Piece
 * @brief Classe représentant une pièce collectable par le joueur.
 */
Piece::Piece(float x, float y) : ObjetInteractif("../img/piece.png") {
    if (!texture.loadFromFile("../img/piece.png")) {
        std::cerr << "Erreur chargement texture pièce" << std::endl;
    } 
    sprite.setTexture(texture);
    sprite.setScale(64.0f / texture.getSize().x, 64.0f / texture.getSize().y);
    sprite.setPosition(x, y);
    startPosition = sprite.getPosition();
    std::cout << "Piece créée à : " << startPosition.x << ", " << startPosition.y << std::endl;
    nbPiece++;
}

/**
 * @brief Met à jour l'animation de la pièce.
 * @param deltaTime Temps écoulé depuis la dernière frame.
 */
void Piece::update(float deltaTime) {
    if (animating) {
        animationTime += deltaTime;

        // Animation de montée/descente
        float offset = animationHeight * std::sin((animationTime / animationDuration) * 3.14159f);
        sprite.setPosition(startPosition.x, startPosition.y - offset);

        if (animationTime >= animationDuration) {
            // Animation terminée, réinitialiser
            sprite.setPosition(startPosition.x, startPosition.y);
            animating = false;
            animationTime = 0.0f;
        }
    }
}

/**
 * @brief Affiche la pièce si elle n'est pas collectée.
 * @param window Fenêtre de rendu.
 */
void Piece::draw(sf::RenderWindow& window) const {
    if (!collected) {
        window.draw(sprite);
    }
}

int Piece::nbPiece = 0; // Initialisation de la variable statique

/**
 * @brief Retourne le nombre de pièces collectées.
 * @return Nombre de pièces collectées.
 */
int Piece::getNbPiece() {
    return nbPiece;
}

/**
 * @brief Réinitialise le compteur de pièces collectées à zéro.
 */
void Piece::resetNbPiece(){
    nbPiece = 0;
}
