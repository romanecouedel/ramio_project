#include "Confetti.h"

Confetti::Confetti(float x, float y, float speed) : speed(speed) {
    shape.setSize(sf::Vector2f(5.f, 5.f));
    shape.setFillColor(sf::Color::White);
    shape.setPosition(x, y);
}

void Confetti::update(float deltaTime) {
    if (!landed) {
        shape.move(0, speed * deltaTime);
    }
}

void Confetti::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Confetti::hasLanded() const {
    return landed;
}

sf::FloatRect Confetti::getBounds() const {
    return shape.getGlobalBounds();
}

void Confetti::stop() {
    landed = true;
}
