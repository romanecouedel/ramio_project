// Bloc.cpp
#include "Bloc.h"
#include <iostream>

// ======================== Classe Bloc ========================
Bloc::Bloc(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement texture : " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(32.0f / texture.getSize().x, 32.0f / texture.getSize().y);
}

// ======================== BlocSol ========================
BlocSol::BlocSol() : Bloc("../img/block.png") {}

// ======================== BlocMystere ========================
BlocMystere::BlocMystere() : Bloc("../img/mystery_box.png") {}
