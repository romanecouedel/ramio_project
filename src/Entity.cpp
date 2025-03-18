#include "Entity.h"

// ====================================== Classe Entity =======================================
// constructeur explicit
// param : le chemin de la texture de l'entité
Entity::Entity(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement texture : " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
}


// retourne le rectangle englobant de l'entité
sf::FloatRect Entity::getGlobalBounds() const {
    return sprite.getGlobalBounds(); // utilisé pour faire les collisions entre entité
}


// ====================================== ObjetInteractif ======================================
// constructeur
// vraiment utile ?
ObjetInteractif::ObjetInteractif(const std::string& texturePath) : Entity(texturePath) {
    sprite.setScale(0.5f, 0.5f);
}

// ========================================= Drapeau ===========================================
// constructeur
Drapeau::Drapeau() : ObjetInteractif("../img/flag.png") {
    sprite.setScale(0.25f, 0.25f); 
    
}

// =========================================== Piece ===========================================
// constructeur
// param : la position de la pièce + chemin texture
// permet de compter le nombre de pièces ont été créées, donc récoltées
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

// animation de la pièce
// param : le temps écoulé depuis la dernière frame
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

// affichage de la pièce
// param : la fenêtre de rendu
void Piece::draw(sf::RenderWindow& window) const {
    if (!collected) {
        window.draw(sprite);
    }

}

int Piece::nbPiece = 0; // Initialisation de la variable statique

// retourne le nombre de pièces collectées
int Piece::getNbPiece() {
    return nbPiece;
}

// remet le nb de pièces à zéro
void Piece::resetNbPiece(){
    nbPiece = 0;
}