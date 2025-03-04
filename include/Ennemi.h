#ifndef ENNEMI_H
#define ENNEMI_H

#include <SFML/Graphics.hpp>

class Ennemi {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    float speed;
    bool movingRight;

public:
    Ennemi(); 
    bool loadTexture(const std::string &filename); 
    void setPosition(float x, float y);
    void update(float deltaTime);
    void draw(sf::RenderWindow &window);
    sf::FloatRect getHitbox() const;
};

#endif // ENNEMI_H
