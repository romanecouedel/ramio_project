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
    bool faceRight = true;
    bool isJumping = false;
    bool canJump = true;

public:
    Player();
    virtual void handleInput() = 0; // Méthode pure virtuelle
    virtual void update(float deltaTime) override;  
    virtual void update(float deltaTime, const Level& level); 
    void draw(sf::RenderWindow& window) const override;
    void jump();
    sf::FloatRect getHitbox() const;
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

    Luigi();
    Luigi(bool aiMode, Level* lvl); 
    void handleInput() override;
    void update(float deltaTime, const Level& level); 
};
