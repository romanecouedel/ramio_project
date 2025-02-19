// Entity.cpp
#include "Entity.h"
#include <iostream>
#include <math.h>

Entity::Entity(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement texture : " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
}

void Entity::move(float dx, float dy) {
    sprite.move(dx, dy);
}

sf::FloatRect Entity::getGlobalBounds() const {
    return sprite.getGlobalBounds();  // Exemple d'implémentation
}


// ===================== ObjetInteractif =====================
ObjetInteractif::ObjetInteractif(const std::string& texturePath) : Entity(texturePath) {
    sprite.setScale(0.5f, 0.5f);
}

// ======================== Constructeur Drapeau ========================
Drapeau::Drapeau() : ObjetInteractif("../img/flag.png") {
    sprite.setScale(0.25f, 0.25f); 
    
}

// ======================== Constructeur Piece ========================

//ajouter code
Piece::Piece(float x, float y) : ObjetInteractif("../img/piece.png") {
    if (!texture.loadFromFile("../img/piece.png")) {
        std::cerr << "Erreur chargement texture pièce" << std::endl;
    } 
    sprite.setTexture(texture);
    sprite.setScale(64.0f / texture.getSize().x, 64.0f / texture.getSize().y);
    sprite.setPosition(x, y);
    startPosition = sprite.getPosition();
    std::cout << "Piece créée à : " << startPosition.x << ", " << startPosition.y << std::endl;

    
}


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

void Piece::draw(sf::RenderWindow& window) const {
    if (!collected) {
        window.draw(sprite);
    }
    //std::cout << "Opacité de la pièce : " << (int)sprite.getColor().a << std::endl;

}
