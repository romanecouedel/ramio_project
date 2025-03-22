#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Level.h"
#include "Player.h"
#include "Menu.h"
#include "FinJeu.h"
#include "Audio.h"

// Définition des différents états possibles du jeu
enum class GameState
{
    MENU, // État du menu principal
    GAME, // État du jeu en cours
    FDG   // État de fin de jeu
};

int main()
{
    // Initialisation des variables
    bool luigiAI = false;         // Si Luigi est contrôlé par l'IA
    bool multijoueur = false;     // Mode multijoueur activé ?
    int nbMortsMario = 0;
    int nbMortsLuigi = 0;

    AudioManager audioManager;
    GameState gameState = GameState::MENU; // Initialisation de l'état du jeu
    sf::RenderWindow window(sf::VideoMode(900, 600), "Mario - Menu");

    // Initialisation des composants du jeu
    Menu menu(900, 600);
    Level level;
    Mario mario;
    Luigi luigi;
    FinDeJeu finDeJeu(window.getSize().x, window.getSize().y);
    sf::Clock clock; // Horloge pour gérer le temps écoulé
    sf::View view(sf::FloatRect(0, 0, 900, 600)); // Vue de la caméra

    // Positions initiales
    sf::Vector2f startPositionMario(100, 100);
    sf::Vector2f startPositionLuigi(150, 100);
    float deltaTime;

    // Boucle principale du jeu
    while (window.isOpen())
    {
        sf::Event event;

        //========================= Gestion des événements =========================
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (gameState == GameState::MENU)
            {
                menu.handleInput(event, window, luigiAI, multijoueur);

                if (audioManager.getCurrentMusic() != "menu")
                    audioManager.playMenuMusic();

                if (menu.isGameStarting())
                {
                    gameState = GameState::GAME;
                    int selectedLevel = menu.getSelectedLevel();
                    std::string levelPath = "../levels/level" + std::to_string(selectedLevel + 1) + ".txt";

                    if (!level.loadFromFile(levelPath))
                    {
                        std::cerr << "Erreur chargement niveau !" << std::endl;
                        return -1;
                    }

                    audioManager.playGameMusic();
                    mario.setPosition(startPositionMario.x, startPositionMario.y);
                    if (multijoueur)
                        luigi.setPosition(startPositionLuigi.x, startPositionLuigi.y);

                    // // Ajuster la taille de la fenêtre en fonction du niveau
                    float blockSize = 64.0f;
                    unsigned int windowWidth = level.getWidth() * blockSize;
                    unsigned int windowHeight = level.getHeight() * blockSize;
                    window.create(sf::VideoMode(windowWidth, windowHeight), "Mario");
                    clock.restart();
                }
            }
            else if (gameState == GameState::FDG)
            {
                if (finDeJeu.handleInput(event))
                {
                    gameState = GameState::MENU;
                    window.create(sf::VideoMode(900, 600), "Mario - Menu");
                    view.setCenter(450, 300);
                    window.setView(view);
                    menu.reset();
                    Piece::resetNbPiece();
                    nbMortsMario = 0;
                    nbMortsLuigi = 0;
                    finDeJeu.victoire = false;
                }
            }
        }

        //========================= Mise à jour et affichage =======================
        if (gameState == GameState::MENU)
        {
            window.clear();
            menu.draw(window);
            window.display();
        }
        else if (gameState == GameState::GAME)
        {
            deltaTime = clock.restart().asSeconds();

            // Mise à jour des personnages
            mario.handleInput();
            mario.update(deltaTime, level);
            level.handleTuyauInteraction(mario, &luigi, deltaTime);

            if (multijoueur)
            {
                level.handleTuyauInteraction(mario, nullptr, deltaTime);
                if (luigiAI)
                    luigi.handleInputAI(&level, &mario);
                else
                    luigi.handleInput();

                luigi.update(deltaTime, level);
            }


            // Vérification du drapeau pour terminer le niveau
            if ((mario.getGlobalBounds().intersects(level.getDrapeau().getGlobalBounds()) || 
                 (multijoueur && luigi.getGlobalBounds().intersects(level.getDrapeau().getGlobalBounds()))))
            {
                std::cout << "Niveau terminé !" << std::endl;
                mario.setPosition(startPositionMario.x, startPositionMario.y);
                if (multijoueur)
                    luigi.setPosition(startPositionLuigi.x, startPositionLuigi.y);

                finDeJeu.victoire = true;
                gameState = GameState::FDG;
                audioManager.playEndMusic(finDeJeu.victoire);
                window.create(sf::VideoMode(900, 600), "Mario - Fin de niveau");
            }

            // Gestion des vies et des morts
            if (mario.isDead) 
            {
                mario.respawn();
                nbMortsMario++;
            }
            if (luigi.isDead)
            {
                luigi.respawn();
                nbMortsLuigi++;
            }

            // Fin de jeu si trop de morts
            if (nbMortsMario + nbMortsLuigi > 2)
            {
                gameState = GameState::FDG;
                window.create(sf::VideoMode(900, 600), "Mario - Fin de niveau");
            }

            // Suivi de Mario et Luigi par la caméra
            sf::Vector2f posMario = mario.getPosition();
            sf::Vector2f posLuigi = multijoueur ? luigi.getPosition() : sf::Vector2f(0, 0);
            float playerPosX = multijoueur ? std::max(posMario.x, posLuigi.x) : posMario.x;

            float levelWidth = level.getWidth() * 64.0f;
            float halfScreenWidth = window.getSize().x / 2.0f;
            float minX = halfScreenWidth;
            float maxX = levelWidth - halfScreenWidth;
            float centerX = std::max(minX, std::min(playerPosX, maxX));
            view.setSize(window.getSize().x, window.getSize().y);
            view.setCenter(centerX, window.getSize().y / 2);
            window.setView(view);

            // Mise à jour et dessin du niveau et des personnages
            if (multijoueur)
                level.update(deltaTime, window, mario.getGlobalBounds(), luigi.getGlobalBounds());
            else
                level.update(deltaTime, window, mario.getGlobalBounds(), sf::FloatRect());

            level.draw(window);
            mario.draw(window);
            if (multijoueur)
                luigi.draw(window);

            window.display();
            window.clear();
        }
        else if (gameState == GameState::FDG)
        {
            finDeJeu.afficher(window, deltaTime, Piece::getNbPiece(), nbMortsMario + nbMortsLuigi);
            window.display();
        }
    }

    return 0;
}
