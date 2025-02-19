// Player.h
#include "Entity.h"
#include "Level.h"
#include "Animation.h"

#include "Animation.h"


class Player : public Entity {
private:
    float jumpForce = -500.f;
    Animation animationWalkLeft;
    Animation animationWalkRight;
    Animation animationJumpRight;
    Animation animationJumpLeft;
    Animation* currentAnimation;
    sf::Texture texture;
    bool faceRight = true;
    bool isJumping = false;
    

public:
    Player();
    void handleInput();
    void update(float deltaTime, const Level& level);
    void update(float deltaTime) override;      // Polymorphisme
    void draw(sf::RenderWindow& window) const override;
    void jump();
    sf::FloatRect getHitbox() const;
};
