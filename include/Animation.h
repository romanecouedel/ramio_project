#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Animation
{
public:
    // Constructeurs
    Animation() = default;
    ~Animation() {
        //std::cout << "Destruction de Animation" << std::endl;
    }
    Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime);

    // Méthodes
    void update(float deltaTime, bool faceRight, bool isJumping);
    sf::IntRect getUvRect() const { return uvRect; }

    // Ajout de la méthode manquante
    sf::IntRect getCurrentFrame() const { return uvRect; }
    sf::Vector2u currentImage;
    sf::IntRect uvRect;
    sf::Vector2u imageCount; 

private:
    // Attributs

    float totalTime = 0.0f;
    float switchTime;
};

#endif
