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
bool luigiAI = false;

int main()
{
    luigiAI = false;          // Définition de la variable
    bool multijoueur = false; // Définit si on joue à 1 ou 2 joueurs
    bool niveauTermine = false;
    AudioManager audioManager;
    int nbMortsMario = 0;
    int nbMortsLuigi = 0;
    int musicmenu = 0;
    // Initialisation de l'état du jeu à MENU
    GameState gameState = GameState::MENU;
    // Création de la fenêtre de jeu avec une résolution de 900x600
    sf::RenderWindow window(sf::VideoMode(900, 600), "Mario - Menu");
    // Initialisation des différents composants du jeu
    Menu menu(900, 600);
    Level level;
    Mario mario;
    Luigi luigi;

    sf::Clock clock; // Horloge pour gérer le temps écoulé
    FinDeJeu finDeJeu(window.getSize().x, window.getSize().y);
    float deltaTime;                              // Temps écoulé entre chaque frame
    sf::View view(sf::FloatRect(0, 0, 900, 600)); // Vue de la caméra

    // Positions de départ pour Mario et Luigi
    sf::Vector2f startPositionMario(100, 100);
    sf::Vector2f startPositionLuigi(150, 100);

    // Boucle principale du jeu
    while (window.isOpen())
    {
        sf::Event event;
        //=============================== Gestion des événements =====================================
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (gameState == GameState::MENU)
            {
                menu.handleInput(event, window);
                if (audioManager.getCurrentMusic() != "menu")
                {
                    audioManager.playMenuMusic();
                }
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
                    multijoueur = menu.isMultiplayerSelected();
                    audioManager.playGameMusic();

                    // Positionner Mario et Luigi au début du niveau
                    mario.setPosition(startPositionMario.x, startPositionMario.y);
                    if (multijoueur)
                        luigi.setPosition(startPositionLuigi.x, startPositionLuigi.y);

                    // Ajuster la taille de la fenêtre en fonction du niveau
                    float blockSize = 64.0f;
                    unsigned int windowWidth = level.getWidth() * blockSize;
                    unsigned int windowHeight = level.getHeight() * blockSize;
                    window.create(sf::VideoMode(windowWidth, windowHeight), "Mario");

                    level.initTexte();
                    niveauTermine = false;
                    clock.restart();
                }
            }

            if (gameState == GameState::FDG)
            {
                if (finDeJeu.handleInput(event))
                {
                    gameState = GameState::MENU;
                    window.create(sf::VideoMode(900, 600), "Mario - Menu");
                    view.setCenter(450, 300);
                    window.setView(view);
                }
            }
        }

        //=============================== Mise à jour et affichage =====================================
        if (gameState == GameState::MENU)
        {
            window.clear();
            menu.draw(window);
            window.display();
        }
        else if (gameState == GameState::GAME)
        {

            deltaTime = clock.restart().asSeconds();

            mario.handleInput();
            mario.update(deltaTime, level);

            if (multijoueur && !luigiAI)
            {
                luigi.handleInput();
            }
            else if (multijoueur && luigiAI)
            {
                luigi.handleAI(deltaTime, mario, level);
            }

            luigi.update(deltaTime, level);

            // Vérifier si le joueur a atteint le drapeau
            if ((mario.getHitbox().intersects(level.getDrapeau().getGlobalBounds()) ||
                 (multijoueur && luigi.getHitbox().intersects(level.getDrapeau().getGlobalBounds()))) &&
                !niveauTermine)
            {
                std::cout << "Niveau terminé !" << std::endl;
                niveauTermine = true;
                // Réinitialisation des positions de Mario et Luigi
                mario.setPosition(startPositionMario.x, startPositionMario.y);
                if (multijoueur)
                    luigi.setPosition(startPositionLuigi.x, startPositionLuigi.y);
                finDeJeu.victoire = true;
                gameState = GameState::FDG;
                audioManager.playEndMusic(finDeJeu.victoire);
                window.create(sf::VideoMode(900, 600), "Mario - Fin de niveau");
            }
            if (!mario.isAlive())
            {
                mario.respawn();
                nbMortsMario++;
            }

            if (!luigi.isAlive())
            {
                luigi.respawn();
                nbMortsLuigi++;
            }
            if ((nbMortsLuigi + nbMortsMario) > 2)
            {
                finDeJeu.victoire = false;
                gameState = GameState::FDG;
                window.create(sf::VideoMode(900, 600), "Mario - Fin de niveau");
            }

            // Suivi de mario et luigi par la caméra
            sf::Vector2f posMario = mario.getPosition();
            sf::Vector2f posLuigi = multijoueur ? luigi.getPosition() : sf::Vector2f(0, 0);
            float playerPosX = multijoueur ? std::max(posMario.x, posLuigi.x) : posMario.x;

            float levelWidth = level.getWidth() * 64.0f;
            float halfScreenWidth = window.getSize().x / 2.0f;
            float minX = halfScreenWidth;
            float maxX = levelWidth - halfScreenWidth;
            float centerX = std::clamp(playerPosX, minX, maxX);

            view.setSize(window.getSize().x, window.getSize().y);
            view.setCenter(centerX, window.getSize().y / 2);
            window.setView(view);

            if (multijoueur)
                level.update(deltaTime, window, mario.getHitbox(), luigi.getHitbox());
            else
                level.update(deltaTime, window, mario.getHitbox(), sf::FloatRect());

            level.draw(window);
            mario.draw(window);

            if (multijoueur)
                luigi.draw(window);

            window.display();
            window.clear();
        }

        else if (gameState == GameState::FDG)
        {
            finDeJeu.afficher(window, deltaTime, Piece ::getNbPiece(), nbMortsLuigi + nbMortsMario);
            window.display();
            //audioManager.playEndMusic(finDeJeu.victoire);
        }
    }

    return 0;
}