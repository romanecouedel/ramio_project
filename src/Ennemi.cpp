#include "Ennemi.h"
#include <iostream>

Ennemi::Ennemi() : speed(50.0f), movingRight(true) {
    if (!texture.loadFromFile("../img/goomba.png")) {
        std::cerr << "Erreur : Impossible de charger koopa.png" << std::endl;
    } 
    sprite.setTexture(texture); 
    sprite.setScale(0.2f, 0.2f);  
}


bool Ennemi::loadTexture(const std::string &filename) {
    if (!texture.loadFromFile(filename)) {
        return false;
    }
    sprite.setTexture(texture);
    return true;
}
void Ennemi::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Ennemi::update(float deltaTime) {
    float moveDistance = speed * deltaTime;
    if (movingRight) {
        sprite.move(moveDistance, 0);
    } else {
        sprite.move(-moveDistance, 0);
    }

    // Changer de direction si un mur est touché
    if (sprite.getPosition().x < 0 || sprite.getPosition().x > 1280 - sprite.getGlobalBounds().width) {
        movingRight = !movingRight;
        sprite.scale(-1.f, 1.f); // Inverser l'orientation
    }
}

void Ennemi::draw(sf::RenderWindow &window) {
    window.draw(sprite);
}

sf::FloatRect Ennemi::getHitbox() const {
    return sprite.getGlobalBounds();
}
