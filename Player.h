// Player.h
#include "Entity.h"
#include "Level.h"

class Player : public Entity {
private:
    
    float jumpForce = -500.f; 
    float speed = 200.f;

public:
    Player();
    ~Player() override {};  // Déclaration du destructeur virtuel
    void handleInput();
    void update(float deltaTime) override;      // <- Polymorphisme
    void draw(sf::RenderWindow& window) const override;
    void update_collision(float deltaTime, const Level& level) override;
    void jump();
    
};
