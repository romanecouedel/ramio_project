// Entity.cpp
#include "Entity.h"
#include <iostream>

Entity::Entity(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement texture : " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
}

Entity::~Entity() {
    // Destructeur virtuel
}

void Entity::move(float dx, float dy) {
    sprite.move(dx, dy);
}



sf::FloatRect Entity::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::FloatRect Entity::getHitbox() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return bounds;
}


// ===================== ObjetInteractif =====================
ObjetInteractif::ObjetInteractif(const std::string& texturePath) : Entity(texturePath) {
    sprite.setScale(0.5f, 0.5f);
}

ObjetInteractif::~ObjetInteractif() {
    // Destructeur virtuel
}

// ======================== Constructeur Drapeau ========================
Drapeau::Drapeau() : ObjetInteractif("../img/flag.png") {
    sprite.setScale(0.5f, 0.5f); 
    
}