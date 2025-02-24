#ifndef CONFETTI_H
#define CONFETTI_H

#include <SFML/Graphics.hpp>

class Confetti {
public:
    Confetti(float x, float y, float speed);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool hasLanded() const;
    sf::FloatRect getBounds() const;
    void stop();

private:
    sf::RectangleShape shape;
    float speed;
    bool landed = false;
};

#endif
