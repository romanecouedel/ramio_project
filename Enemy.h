#include "Entity.h"
#include "Level.h"

class Enemy : public Entity {
protected: 
    float speed = 100.f;
    bool direction = true;
    float distance = 0.f; 
    float maxDistance = 200.f;
    bool canJump = false;
    bool onGround = false;


public:
    Enemy(){};
    ~Enemy() override{} ;  // Déclaration du destructeur virtuel
    virtual void update(float deltaTime) = 0;
    void draw(sf::RenderWindow& window) const override;
    void update_collision(float deltaTime, const Level &level) override;

    
    
};


/*class Goomba : public Enemy {
protected:
    bool isSquished = false;
    float squishTimer = 0.f;
    const float squishDuration = 0.5f;
};*/
