#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Entity.h"

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

// ======================== Bloc Mystère ========================
class BlocMystere : public Bloc {
public:
    BlocMystere();
    void update(float deltaTime, sf::RenderWindow& window);
    void onHit();
    bool isAnimating() const;
    bool estTouche = false;
    std::unique_ptr<Piece>& getPiece() { return piece; }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    

private:
    sf::Vector2f startPosition;
    bool animating = false;
    float animationTime = 0.0f;
    const float animationDuration = 0.2f;
    const float animationHeight = 5.0f;

    std::unique_ptr<Piece> piece;

    sf::Texture textureFoncee;

    void changerTexture();
};
