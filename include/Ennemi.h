#ifndef ENNEMI_H
#define ENNEMI_H

#include "Entity.h"
#include "Level.h"
#include <SFML/Graphics.hpp>

class Level; 
class Ennemi : public Entity
{
public:
    Ennemi();

    void setPosition(float x, float y);
    void update(float deltaTime, class Level &level);
    void onPlayerCollision(bool fromAbove);
    void draw(sf::RenderWindow &window) const override;
    sf::FloatRect getBounds() const;
    bool isAlive = true; 
    
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool isSquashed;
    float squashTimer = 0.f; 
    float animationTimer = 0.f;
    int currentFrame = 0;

    float gravity = 1000.f; 
    bool onGround = false;     

    static sf::Texture textureEnnemi;
    int frameWidth, frameHeight;
};

#endif
