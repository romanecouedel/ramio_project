#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

// ========================== Classe mère ==========================
class Bloc {
protected:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    Bloc() = default;
    Bloc(const std::string& texturePath); 
    virtual ~Bloc() = default;

    virtual void draw(sf::RenderWindow& window) const { 
        window.draw(sprite); 
    }

    sf::FloatRect getGlobalBounds() const {
        return sprite.getGlobalBounds();
    }

    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }
};

// ======================== Blocs Spécifiques ========================

// Bloc de Sol
class BlocSol : public Bloc {
public:
    BlocSol();
};

// Bloc Mystère
class BlocMystere : public Bloc {
public:
    BlocMystere();
};

