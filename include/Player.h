#pragma once
#include "Entity.h"
#include "Animation.h"
#include <cstdlib>
#include <ctime>

class Level;


// ============================================= Classe Player ===========================================$
// classe fille de Entity
// classe mère de mario et luigi

class Player : public Entity {
protected:
    // variables d'animation uniquement accessibles par les classes filles
    Animation animationWalkLeft;
    Animation animationWalkRight;
    Animation animationJumpRight;
    Animation animationJumpLeft;
    Animation animationIdleRight;
    Animation animationIdleLeft;
    Animation* currentAnimation;
    sf::Vector2f velocity;
    float jumpForce = -650.f;
    float speed = 350.f;
    float gravity = 980.f;

    // variables de saut, nottament utilisée pour l'ia de luigi
    bool isJumping = false;
    bool canJump = true;
    bool onGround = false;


public:
    // constructeur
    Player();
    bool faceRight = true; // Direction du personnage, sert pour l'animation
    bool isDead = false;// sert à compter les morts
    bool collisionsActive = true; // Désactive la collision temporairement
    
    void draw(sf::RenderWindow& window) const override;// surchargée car méthode héritée de Entity
    void jump();
    void move(float dx, float dy) {sprite.move(dx, dy);}
    void initializePlayer(const std::string& texturePath, sf::Vector2f position);    
    void respawn();

    void setCollisionsActive(bool active) { collisionsActive = active; }
    bool areCollisionsActive() const { return collisionsActive; }
    void setOpacity(sf::Uint8 alpha); // sert pour l'animation tuyau
    void update(float deltaTime, const Level& level); // mise à jour du joueur en fonction des collisions du level


    // Méthodes virtuelles pures, implémentées par les classes filles
    virtual void handleInput() = 0; 
};

// ============================================= Classe Mario ===========================================
// classe fille de Player
// représente le personnage Mario jouable uniquement par l'utilisateur
class Mario : public Player {
public:
    // constructeur
    Mario();
    // entrées clavier pour déplacer Mario
    void handleInput() override; //polymorphisme car peut etre utilisé par luigi ou mario et faire des actions différentes
};

// ============================================= Classe Luigi ===========================================
// classe fille de Player
// représente le personnage Luigi pour un jeu 2 joueurs en local ou avec l'ordinateur
class Luigi : public Player {
public:
    Level* level;
    const Mario* mario; // Référence à Mario pour l'IA

    // constructeur
    Luigi(); 

    // entrées clavier pour déplacer Luigi en local
    void handleInput() override;//polymorphisme car peut etre utilisé par luigi ou mario et faire des actions différentes

    // déplacement de Luigi en fonction de Mario pour l'IA
    void marcher_normal();
    void handleInputAI(Level* lvl, const Mario* mario);

};

