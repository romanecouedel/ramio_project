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


// Classe gère les levels du jeu
class Level
    {
    public:
    // Constructeur
        Level();
        // Destructeur
        ~Level() = default;

        const Drapeau &getDrapeau() const { return drapeau; }

        std::vector<Ennemi> ennemis;

        // Chargement du niveau depuis un fichier txt
        bool loadFromFile(const std::string &filename);
        // Dessin du niveau
        void draw(sf::RenderWindow &window);
        // gestion des collisions
        bool isColliding(const sf::FloatRect &hitbox) const;

        // utile pour ia, savoir si il y a un bloc mystere près de luigi
        BlocMystere* getBlocMystereProche(const sf::Vector2f& position);
        
        // utile pour ia, afficher les blocs mysteres et leur état, DEBUG
        void afficherEtatBlocsMysteres() const ;


        int getWidth() const { return grid.empty() ? 0 : grid[0].size(); }
        int getHeight() const { return grid.size(); }

        // Mise à jour du niveau
        void update(float deltaTime, sf::RenderWindow &window, const sf::FloatRect &marioBounds, const sf::FloatRect &luigiBounds);


        void generateBackground(float levelWidth, float levelHeight);

        bool isTuyauColliding(const sf::FloatRect& hitbox) const;

        void updateEnnemis(float deltaTime);
        void drawEnnemis(sf::RenderWindow &window);

        void handleTuyauInteraction(Player &player, float deltaTime);


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


        float tuyauTimer = 0.0f; // Temps écoulé pour l'animation
        bool enTrainDeDescendre = false; // Animation de descente active
        bool enTrainDeMonter = false; // Animation de montée active
        sf::Vector2f sortiePosition; // Position du tuyau de sortie


};

#endif
