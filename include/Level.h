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
        // Génération du fond avec défilement
        void generateBackground(float levelWidth, float levelHeight);
        // gestion des collisions
        bool isColliding(const sf::FloatRect &hitbox) const;

        // utile pour ia, savoir si il y a un bloc mystere près de luigi
        BlocMystere* getBlocMystereProche(const sf::Vector2f& position);
        // utile pour ia, afficher les blocs mysteres et leur état, DEBUG
        void afficherEtatBlocsMysteres() const ;

        // taille de la grille, combien de caractere dans une ligne dans le fichier txt, utile pour main
        int getWidth() const { return grid.empty() ? 0 : grid[0].size(); }
        int getHeight() const { return grid.size(); }

        // Mise à jour du niveau
        void update(float deltaTime, sf::RenderWindow &window, const sf::FloatRect &marioBounds, const sf::FloatRect &luigiBounds);

        // Gestion des tuyaux
        bool isTuyauColliding(const sf::FloatRect& hitbox) const;
        void handleTuyauInteraction(Player &player, float deltaTime);

        // Gestion des ennemis
        void updateEnnemis(float deltaTime);
        void drawEnnemis(sf::RenderWindow &window);


    private:
        // vecteur de vecteur d'entiers pour la grille
        std::vector<std::vector<int>> grid;
        std::vector<std::unique_ptr<Bloc>> blocs; // Vecteur de blocs

        Drapeau drapeau;
        
        // Fond
        sf::Texture backgroundTexture;
        sf::Sprite backgroundSprite;
        sf::Texture bgTextureLeft, bgTextureRight;
        sf::VertexArray backgroundVertices;
        float bgWidth;

        sf::Music backgroundMusic;

        // Tuyau
        sf::RectangleShape tuyauEntree;
        sf::RectangleShape tuyauSortie;
        float tuyauTimer = 0.0f; // Temps écoulé pour l'animation
        bool enTrainDeDescendre = false; // Animation de descente active
        bool enTrainDeMonter = false; // Animation de montée active
        sf::Vector2f sortiePosition; // Position du tuyau de sortie


};

#endif
