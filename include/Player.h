#pragma once
#include "Entity.h"
#include "Animation.h"
#include <cstdlib>
#include <ctime>

class Level;
class Player : public Entity {
protected:
    float jumpForce = -650.f;
    Animation animationWalkLeft;
    Animation animationWalkRight;
    Animation animationJumpRight;
    Animation animationJumpLeft;
    Animation animationIdleRight;
    Animation animationIdleLeft;
    Animation* currentAnimation;
    sf::Texture texture;
    bool isJumping = false;
    bool canJump = true;


public:
    Player();
    bool faceRight = true;
    bool isDead = false;
    bool visible = true;
    bool collisionsActive = true; // Désactive la collision temporairement
    virtual void handleInput() = 0; // Méthode pure virtuelle
    virtual void update(float deltaTime) override;  
    virtual void update(float deltaTime, const Level& level); 
    void draw(sf::RenderWindow& window) const override;
    bool isAlive() const { return !isDead; } 
    void jump();
    sf::FloatRect getHitbox() const;
    sf::Vector2f getPosition() const;
    void setCollisionsActive(bool active) { collisionsActive = active; }
    bool areCollisionsActive() const { return collisionsActive; }
    void setOpacity(sf::Uint8 alpha);
    void respawn();
    

};

class Mario : public Player {
public:
    Mario();
    void handleInput() override;
    void respawn(); 
    void update(float deltaTime, const Level& level); 
};

class Luigi : public Player {
public:
    bool isAI; // Indique si Luigi est contrôlé par l'IA
    Level* level;
    const Mario* mario; // Référence à Mario
    sf::Clock waitClock; // Horloge pour mesurer le temps d'attente
    bool waiting = false; // Indique si Luigi est en attente



   

    Luigi(); 
    void handleInput() override;
    void update(float deltaTime, const Level& level); 
    void marcher_normal();
    void handleInputAI(Level* lvl, const Mario* mario);
    void respawn();

    bool interruption_mystere = false;


};

