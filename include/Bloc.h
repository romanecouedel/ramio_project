#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Entity.h"
#include "Player.h"


class Player;

// ========================== Classe mère ==========================
class Bloc {
protected:
    sf::Sprite sprite;
    static sf::Texture textureBlocSol;  // Texture partagée pour BlocSol
    static sf::Texture textureBlocMystere; // Texture partagée pour BlocMystere
    static sf::Texture textureTuyau;
 
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
    sf::Vector2f getPosition() const { 
        return sprite.getPosition();
    }

    const sf::Sprite& getSprite() const { 
        return sprite;
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

// ======================== Tuyau ========================
class Tuyau : public Bloc {
    public:
        enum class Type { ENTREE, SORTIE };
        Tuyau(Type type);
        Type getType() const;
        bool isPlayerOnTop(const Player& player) const; // Ajout de la déclaration
        Tuyau* getSortieAssociee(const std::vector<std::unique_ptr<Bloc>>& blocs) const;
    private:
        Type type;
    };
    