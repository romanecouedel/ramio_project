#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Bloc.h"
#include "Entity.h"
#include "Confetti.h"

class Level
{
public:
    Level();
    ~Level() = default;

    bool loadFromFile(const std::string &filename);
    void draw(sf::RenderWindow &window);
    bool isColliding(const sf::FloatRect &hitbox) const;

    int getWidth() const { return grid.empty() ? 0 : grid[0].size(); }
    int getHeight() const { return grid.size(); }

    const Drapeau &getDrapeau() const { return drapeau; }
    void update(float deltaTime, sf::RenderWindow &window, const sf::FloatRect &playerBounds);

    // Confetti
    void startConfetti();
    bool confettiActive = false;

    void initTexte();
    bool afficherTexte = false; // Variable pour contrôler l'affichage

    void spawnPiece(const sf::Vector2f& position);
    void updatePieces(float deltaTime, const sf::FloatRect& playerHitbox);
    void drawPieces(sf::RenderWindow& window);

private:
    std::vector<std::vector<int>> grid;
    std::vector<std::unique_ptr<Bloc>> blocs; // Vecteur de blocs
    Drapeau drapeau;
    // Animation fin

    float confettiTimer = 0.0f;
    const float confettiDuration = 5.0f;
    std::vector<Confetti> confettis;
    std::vector<sf::FloatRect> confettiStack;

    sf::Font font;
    sf::Text niveauTermineText;
    bool texteAnime = false;             // Nouveau : indique si l’animation est en cours
    float zoomScale = 1.0f; // Facteur de zoom actuel
    bool zoomIn = true;     // Direction du zoom (avant ou arrière)

    sf::Clock texteClock; 
    bool texteAnimationActive = false;
};

#endif