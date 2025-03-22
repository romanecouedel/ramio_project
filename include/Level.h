#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Bloc.h"
#include "Entity.h"
#include "Player.h"
#include "Ennemi.h"

class Ennemi;
class Level
{
public:
    Level();
    ~Level() = default;

    bool loadFromFile(const std::string &filename);
    void draw(sf::RenderWindow &window);
    bool isColliding(const sf::FloatRect &hitbox) const;
    BlocMystere* getBlocMystereProche(const sf::Vector2f& position);
    
    void afficherEtatBlocsMysteres() const ;




    int getWidth() const { return grid.empty() ? 0 : grid[0].size(); }
    int getHeight() const { return grid.size(); }

    const Drapeau &getDrapeau() const { return drapeau; }
    void update(float deltaTime, sf::RenderWindow &window, const sf::FloatRect &marioBounds, const sf::FloatRect &luigiBounds);

    void initTexte();
    bool afficherTexte = false; // Variable pour contrôler l'affichage

    void generateBackground(float levelWidth, float levelHeight);

    bool isTuyauColliding(const sf::FloatRect& playerBound) const;    

    void updateEnnemis(float deltaTime);
    void drawEnnemis(sf::RenderWindow &window);

    void handleTuyauInteraction(Player &mario, Player *luigi, float deltaTime);

private:
    std::vector<std::vector<int>> grid;
    std::vector<std::unique_ptr<Bloc>> blocs; // Vecteur de blocs

    Drapeau drapeau;
    
    
    sf::Font font;
    sf::Text niveauTermineText;
    bool texteAnime = false;             // Indique si l’animation est en cours
    float zoomScale = 1.0f; // Facteur de zoom actuel
    bool zoomIn = true;     // Direction du zoom (avant ou arrière)

    sf::Clock texteClock;
    bool texteAnimationActive = false;
    
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture bgTextureLeft, bgTextureRight;
    sf::VertexArray backgroundVertices;
    float bgWidth;

    int viesRestantes = 5; // Nombre de vies au début
    sf::Text texteVies; 

    sf::Music backgroundMusic;

    sf::RectangleShape tuyauEntree;
    sf::RectangleShape tuyauSortie;

    sf::Texture goombaTexture;
    std::vector<std::unique_ptr<Ennemi>> ennemis;


    float tuyauTimer = 0.0f; // Temps écoulé pour l'animation
    bool enTrainDeDescendre = false; // Animation de descente active
    bool enTrainDeMonter = false; // Animation de montée active
    sf::Vector2f sortiePosition; // Position du tuyau de sortie


};

#endif
