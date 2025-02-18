// Player.h
#include "Entity.h"
#include "Level.h"

class Player : public Entity {
private:
    
    float jumpForce = -500.f; 

public:
    Player();
    void handleInput();
    void update(float deltaTime, const Level& level);
    void update(float deltaTime) override;      // <- Polymorphisme
    void draw(sf::RenderWindow& window) const override;
    void jump();
    sf::FloatRect getHitbox() const;
};
