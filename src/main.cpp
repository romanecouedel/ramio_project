#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.h"
#include "Player.h"
#include "Menu.h"
#include "FinJeu.h"

enum class GameState
{
    MENU,
    GAME,
    FDG
}; // États possibles du jeu

int main()
{
    GameState gameState = GameState::MENU; // Démarrage dans le menu
    sf::RenderWindow window(sf::VideoMode(900, 600), "Mario - Menu");

    Menu menu(900, 600);
    Level level;
    Mario mario;
    Luigi luigi(true, &level); // true = IA activée
    bool multijoueur = false; // Définit si on joue à 1 ou 2 joueurs

    bool niveauTermine = false;
    sf::Clock clock;
    FinDeJeu finDeJeu(window.getSize().x, window.getSize().y);
    float deltaTime;
    sf::View view(sf::FloatRect(0, 0, 900, 600));

    sf::Vector2f startPositionMario(100, 100);
    sf::Vector2f startPositionLuigi(150, 100);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (gameState == GameState::MENU)
            {
                menu.handleInput(event, window);
                if (menu.isGameStarting())
                { 
                    // Démarrer le jeu
                    gameState = GameState::GAME;
                    int selectedLevel = menu.getSelectedLevel();
                    std::string levelPath = "../levels/level" + std::to_string(selectedLevel + 1) + ".txt";

                    if (!level.loadFromFile(levelPath))
                    {
                        std::cerr << "Erreur chargement niveau !" << std::endl;
                        return -1;
                    }

                    // Vérifier si le joueur a choisi 2 joueurs
                    multijoueur = menu.isMultiplayerSelected();

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

            if (multijoueur)
            {
                luigi.handleInput();
                luigi.update(deltaTime, level);
            }

            if ((mario.getHitbox().intersects(level.getDrapeau().getGlobalBounds()) ||
                 (multijoueur && luigi.getHitbox().intersects(level.getDrapeau().getGlobalBounds()))) &&
                !niveauTermine)
            {
                std::cout << "Niveau terminé !" << std::endl;
                niveauTermine = true;
                mario.setPosition(startPositionMario.x, startPositionMario.y);
                if (multijoueur)
                    luigi.setPosition(startPositionLuigi.x, startPositionLuigi.y);
                gameState = GameState::FDG;
                window.create(sf::VideoMode(900, 600), "Mario - Fin de niveau");
            }

            // Gestion de la caméra
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
            int score = 0;
            finDeJeu.afficher(window, deltaTime, score);
            level.update(deltaTime, window, sf::FloatRect(), sf::FloatRect());
            window.display();
        }
    }

    return 0;
}
