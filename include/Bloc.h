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
    sf::Texture texture;
    BlocType type;

public:
    Bloc() = default; // Constructeur par défaut car on a des classes filles différentes
    explicit Bloc(const std::string& texturePath, BlocType type); // Constructeur avec texture
    virtual ~Bloc() = default; // Destructeur virtuel

    virtual void draw(sf::RenderWindow& window) const { window.draw(sprite); } // Dessine le bloc, virtuel pour les classes filles

    // Retourne les dimensions globales du bloc
    sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); } // Récupère la boite englobante du sprite, utilisée pour les collisions dans player

    // Position
    void setPosition(float x, float y) { sprite.setPosition(x, y); } // Définit la position du bloc
    sf::Vector2f getPosition() const { return sprite.getPosition(); } // Récupère la position du bloc

    // Accès au sprite
    const sf::Sprite& getSprite() const { return sprite; } // Récupère le sprite du bloc

    // Récupère le type du bloc
    BlocType getType() const { return type; }   // Récupère le type du bloc
};


// ======================== Classe BLOC SOL ========================
// Classe fille de Bloc
class BlocSol : public Bloc {
public:
    // Constructeur
    BlocSol();
};


// ======================== Classe BLOC MYSTERE ========================
// Classe fille de Bloc
class BlocMystere : public Bloc {
private:
    // utile pour animation
    sf::Vector2f startPosition;
    bool animating = false;
    float animationTime = 0.0f;
    const float animationDuration = 0.2f;
    const float animationHeight = 5.0f;
    sf::Texture textureFoncee;
    std::unique_ptr<Piece> piece;

    void changerTexture();

public:
    // Constructeur
    BlocMystere();
    // Animation
    void update(float deltaTime, sf::RenderWindow& window);
    void onHit();
    bool isAnimating() const { return animating; }
    bool estTouche = false; // utile pour ia

    std::unique_ptr<Piece>& getPiece() { return piece; }
};


// ======================== Classe TUYAU========================
// Classe fille de Bloc
class Tuyau : public Bloc {
public:
    enum class TypeES { ENTREE, SORTIE };

private:
    TypeES type;

public:
    explicit Tuyau(TypeES type);

    TypeES getType() const { return type; }
    bool isPlayerOnTop(const Player& player) const;
};
