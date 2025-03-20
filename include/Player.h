#pragma once
#include "Entity.h"
#include "Animation.h"
#include <cstdlib>
#include <ctime>
#include <memory> 

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
    std::unique_ptr<sf::Texture> texture; 
    bool isJumping = false;
    bool canJump = true;
    bool isDead = false;


public:
    Player();
    virtual ~Player(); 
    bool faceRight = true;
    virtual void handleInput() = 0; // Méthode pure virtuelle
    virtual void update(float deltaTime) override;  
    virtual void update(float deltaTime, const Level& level); 
    void draw(sf::RenderWindow& window) const override;
    bool isAlive() const { return !isDead; } 
    void jump();
    sf::FloatRect getHitbox() const;
    virtual void respawn(); 

    bool visible = true; // Permet de cacher le joueur progressivement
    bool collisionsActive = true; // Désactive la collision temporairement
    void setVisible(bool v) { visible = v; }
    bool isVisible() const { return visible; }

    void setCollisionsActive(bool active) { collisionsActive = active; }
    bool areCollisionsActive() const { return collisionsActive; }
    void setOpacity(sf::Uint8 alpha);

    
    sf::Vector2f getPosition() const;
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

    void handleInputAI(Level* lvl ,const Mario* mario);
    Luigi(); 
    bool isAIEnabled = true;
    void respawn(); 
    void handleInput() override;
    void update(float deltaTime, const Level& level);
    void marcher_normal();

};

