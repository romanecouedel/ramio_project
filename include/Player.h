// Player.h
#include "Entity.h"
#include "Level.h"

class Player : public Entity {
private:
    sf::Vector2f velocity;
    float speed = 200.f;
    float jumpForce = -500.f;
    float gravity = 980.f;
    bool canJump = false;
    bool onGround = false;

public:
    Player();
    void handleInput();
    void update(float deltaTime, const Level& level);
    void update(float deltaTime) override;      // <- Polymorphisme
    void draw(sf::RenderWindow& window) const override;
    void jump();
    sf::FloatRect getHitbox() const;
};
