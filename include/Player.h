#pragma once
#include "Entity.h"
#include "Animation.h"

// Déclaration avancée de Level
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
    virtual void handleInput() = 0; // Méthode pure virtuelle
    virtual void update(float deltaTime) override;  
    virtual void update(float deltaTime, const Level& level); 
    void draw(sf::RenderWindow& window) const override;
    void jump();
    sf::FloatRect getHitbox() const;
    sf::Vector2f getPosition() const;
};

class Mario : public Player {
public:
    Mario();
    void handleInput() override;
    void update(float deltaTime, const Level& level); 
};

class Luigi : public Player {
public:
    bool isAI;
    Level* level;
    const Mario* mario; // Référence à Mario
    sf::Clock waitClock; // Horloge pour mesurer le temps d'attente
    bool waiting = false; // Indique si Luigi est en attente



   

    Luigi(bool aiMode, Level* lvl ,const Mario* mario); 
    void handleInput() override;
    void update(float deltaTime, const Level& level); 
    void marcher_normal();
    void attendreAvantDeChercherBoite();

    bool interruption_mystere = false;


};
