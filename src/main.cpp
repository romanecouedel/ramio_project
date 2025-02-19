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
    Player player;

    bool niveauTermine = false;
    sf::Clock clock;
    FinDeJeu finDeJeu(window.getSize().x, window.getSize().y); // Écran de fin de jeu
    float deltaTime;
    sf::View view(sf::FloatRect(0, 0, 900, 600));
    sf::Vector2f startPosition(100, 100); // Position de départ (exemple)
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
                { // Vérifier si l'utilisateur a validé "Jouer"
                    gameState = GameState::GAME;

                    // Charger le niveau sélectionné
                    int selectedLevel = menu.getSelectedLevel();
                    std::string levelPath = "../levels/level" + std::to_string(selectedLevel + 1) + ".txt";
                    if (!level.loadFromFile(levelPath))
                    {
                        std::cerr << "Erreur chargement niveau !" << std::endl;
                        return -1;
                    }
                    player.setPosition(startPosition.x, startPosition.y);
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
                // Gérer l'entrée de l'utilisateur
                if (finDeJeu.handleInput(event))
                {
                    gameState = GameState::MENU;

                    window.create(sf::VideoMode(900, 600), "Mario - Menu"); // Réinitialiser la fenêtre
                    view.setCenter(450, 300);                               // Recentrer la vue sur le menu
                    window.setView(view);                                   // Appliquer la vue normale pour le menu
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

            player.handleInput();
            player.update(deltaTime, level);

            if (player.getHitbox().intersects(level.getDrapeau().getGlobalBounds()) && !niveauTermine)
            {
                std::cout << "Niveau terminé !" << std::endl;
                niveauTermine = true;
                player.setPosition(startPosition.x, startPosition.y);
                gameState = GameState::FDG;

                window.create(sf::VideoMode(900, 600), "Mario - Fin de niveau"); // Réinitialiser la fenêtre
            }
            
            // Récupère la position actuelle de Mario
            sf::Vector2f playerPos = player.getPosition();

            // Taille du niveau en pixels
            float levelWidth = level.getWidth() * 64.0f;
            float halfScreenWidth = window.getSize().x / 2.0f;

            // Limiter la position X de la caméra
            float minX = halfScreenWidth;                        // Bord gauche
            float maxX = levelWidth - halfScreenWidth;           // Bord droit
            float centerX = std::clamp(playerPos.x, minX, maxX); // Empêcher la caméra d'aller trop loin

            // Appliquer la vue avec les limites
            view.setSize(window.getSize().x, window.getSize().y);
            view.setCenter(centerX, window.getSize().y / 2);
            window.setView(view);

            level.update(deltaTime, window, player.getHitbox());
            level.draw(window);
            player.draw(window);
            window.display();
            window.clear();
        }

        else if (gameState == GameState::FDG)
        {
            int score = 0;

            finDeJeu.afficher(window, deltaTime, score);

            level.update(deltaTime, window, sf::FloatRect());

            window.display();
            window.clear(sf::Color::Blue);
        }

        else if (gameState == GameState::FDG)
        {
            int score = 0;

            finDeJeu.afficher(window, deltaTime, score);

            level.update(deltaTime, window, sf::FloatRect());

            window.display();
        }
    }

    return 0;
}
