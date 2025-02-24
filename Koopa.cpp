#include "Koopa.h"
#include <iostream>


Koopa::Koopa
() {

    if (!texture.loadFromFile("../img/koopa.png"))
    {
        std::cerr << "Erreur chargement Koopa" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    sprite.setPosition(500, 100);

}

void Koopa::move_ia(){
    
}

void Koopa::update(float deltaTime) {
    if (isShell) {
        shellTimer += deltaTime;
        if (shellTimer >= shellDuration) {
            isShell = false;
            shellTimer = 0.f;
        }
    }
    else {
        if (direction) {
            velocity.x = speed;
        }
        else {
            velocity.x = -speed;
        }
        distance += speed * deltaTime;
        if (distance >= maxDistance) {
            direction = !direction;
            distance = 0.f;
        }
    }
    sprite.move(velocity * deltaTime);
}
