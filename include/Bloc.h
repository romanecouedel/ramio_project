#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Entity.h"

class Player;

enum class BlocType { SOL, MYSTERE, TUYAU};

// ======================================= Classe Bloc ===========================================
// Classe abstraite représentant un bloc du jeu
// classe mère de BlocSol, BlocMystere et Tuyau
class Bloc {
protected:
    sf::Sprite sprite;
    static sf::Texture textureBlocSol;  // Texture partagée pour BlocSol
    static sf::Texture textureBlocMystere; // Texture partagée pour BlocMystere
    static sf::Texture textureTuyau;
 
public: 
    Bloc() = default;
    virtual ~Bloc() {
        //std::cout << "Destruction de Bloc" << std::endl;
    }
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
    // Constructeur
    BlocSol();
    ~BlocSol() override {
        //std::cout << "Destruction de BlocSol" << std::endl;
    }
};


// ======================== Classe BLOC MYSTERE ========================
// Classe fille de Bloc
class BlocMystere : public Bloc {
private:
    bool animating = false;
    float animationTime = 0.0f;
    const float animationDuration = 0.2f;
    const float animationHeight = 5.0f;
     sf::Vector2f startPosition;  // utile pour animation
    sf::Texture textureFoncee;
    std::unique_ptr<Piece> piece;

    void changerTexture();

public:
    // Constructeur
    BlocMystere();
    ~BlocMystere() override {
        //std::cout << "Destruction de BlocMystere" << std::endl;
    }
    // Animation
    void update(float deltaTime, sf::RenderWindow& window);
    void onHit();
    bool isAnimating() const;
    bool estTouche = false; // utile pour ia

    std::unique_ptr<Piece>& getPiece() { return piece; }
};


// ======================== Classe TUYAU========================
// Classe fille de Bloc
class Tuyau : public Bloc {
    public:
        enum class Type { ENTREE, SORTIE };
        Tuyau(Type type);
        ~Tuyau() {
        //std::cout << "Destruction de Tuyau" << std::endl;
        }
        Type getType() const;
        bool isPlayerOnTop(const Player& player) const; // Ajout de la déclaration
        Tuyau* getSortieAssociee(const std::vector<std::unique_ptr<Bloc>>& blocs) const;
    private:
        Type type;

    };
    

// ======================== BLOC EAU ========================
class Eau : public Bloc {
public:
    Eau();  // Constructeur
    bool isPlayerInWater(const sf::FloatRect& playerHitbox) const;
    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds() const;

private:
    static sf::Texture textureEau; 
};
