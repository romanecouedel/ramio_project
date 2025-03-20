#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Bloc.h"
#include "Entity.h"
#include "Player.h"

class Level {
public:
    Level();
    ~Level();

    bool loadFromFile(const std::string &filename);
    void draw(sf::RenderWindow &window);
    bool isColliding(const sf::FloatRect &hitbox) const;
    BlocMystere* getBlocMystereProche(const sf::Vector2f& position);
    
    void afficherEtatBlocsMysteres() const;

    int getWidth() const { return grid.empty() ? 0 : grid[0].size(); }
    int getHeight() const { return grid.size(); }

    const Drapeau &getDrapeau() const { return drapeau; }
    void update(float deltaTime, sf::RenderWindow &window, const sf::FloatRect &marioBounds, const sf::FloatRect &luigiBounds);

    void initTexte();
    bool afficherTexte = false;

    void generateBackground(float levelWidth, float levelHeight);
    void handleTuyauInteraction(Player& mario, Player* luigi, float deltaTime);

private:
    std::vector<std::vector<int>> grid;
    std::vector<std::unique_ptr<Bloc>> blocs;

    Drapeau drapeau;

    sf::Font font;
    sf::Text niveauTermineText;
    bool texteAnime = false;
    float zoomScale = 1.0f;
    bool zoomIn = true;

    sf::Clock texteClock;
    bool texteAnimationActive = false;
    
    // 🔥 On garde les noms d'origine !
    static sf::Texture bgTextureLeft;
    static sf::Texture bgTextureRight;
    
    sf::Sprite backgroundSprite;
    sf::VertexArray backgroundVertices;
    float bgWidth;

    int viesRestantes = 5;
    sf::Text texteVies;

    sf::Music backgroundMusic;

    sf::RectangleShape tuyauEntree;
    sf::RectangleShape tuyauSortie;

    float tuyauTimer = 0.0f;
    bool enTrainDeDescendre = false;
    bool enTrainDeMonter = false;
    sf::Vector2f sortiePosition;
};

#endif
