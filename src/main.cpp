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
    bool luigiAI = false;     // Si Guili est contrôlé par l'IA
    bool multijoueur = false; // Mode multijoueur activé ?
    int nbMortsMario = 0;
    int nbMortsLuigi = 0;

    AudioManager audioManager;
    GameState gameState = GameState::MENU; // Initialisation de l'état du jeu
    sf::RenderWindow window(sf::VideoMode(900, 600), "Ramio - Menu");

    // Initialisation des composants du jeu
    Menu menu(900, 600);
    Level level;
    Ramio ramio;
    Guili guili;
    FinDeJeu finDeJeu(window.getSize().x, window.getSize().y);
    sf::Clock clock;                              // Horloge

    float totalTime = 0.f; // Variable pour accumuler le temps total
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

                    totalTime = 0.f; // remise à zero du temps à chaque fois qu'une partie commence 
                    gameState = GameState::GAME;
                    int selectedLevel = menu.getSelectedLevel();
                    std::string levelPath = "../levels/level" + std::to_string(selectedLevel + 1) + ".txt";

                    if (!level.loadFromFile(levelPath))
                    {
                        std::cerr << "Erreur chargement niveau !" << std::endl;
                        return -1;
                    }

                    audioManager.playGameMusic();
                    ramio.setPosition(startPositionMario.x, startPositionMario.y);
                    if (multijoueur)
                        guili.setPosition(startPositionLuigi.x, startPositionLuigi.y);

                    // // Ajuster la taille de la fenêtre en fonction du niveau
                    float blockSize = 64.0f;
                    unsigned int windowWidth = level.getWidth() * blockSize;
                    unsigned int windowHeight = level.getHeight() * blockSize;
                    window.close();
                    window.create(sf::VideoMode(windowWidth, windowHeight), "Ramio");
                    clock.restart();
                }
            }
            else if (gameState == GameState::FDG)
            {
                if (finDeJeu.handleInput(event))
                {
                    gameState = GameState::MENU;
                    window.close();
                    window.create(sf::VideoMode(900, 600), "Ramio - Menu");
                    view.setCenter(450, 300);
                    window.setView(view);
                    // Réinitialisation des variables
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
            totalTime += deltaTime; // Ajouter le deltaTime au temps total

            // Mise à jour des personnages
            ramio.handleInput();
            ramio.update(deltaTime, level);
            level.handleTuyauInteraction(ramio, &guili, deltaTime);

            if (multijoueur)
            {
                level.handleTuyauInteraction(ramio, nullptr, deltaTime);
                if (luigiAI)
                    guili.handleInputAI(&level, &ramio);
                else
                    guili.handleInput();

                guili.update(deltaTime, level);
            }

            // Vérification du drapeau pour terminer le niveau
            if ((ramio.getGlobalBounds().intersects(level.getDrapeau().getGlobalBounds()) ||
                 (multijoueur && guili.getGlobalBounds().intersects(level.getDrapeau().getGlobalBounds()))))
            {
                std::cout << "Niveau terminé !" << std::endl;
                ramio.setPosition(startPositionMario.x, startPositionMario.y);
                if (multijoueur)
                    guili.setPosition(startPositionLuigi.x, startPositionLuigi.y);

                finDeJeu.victoire = true;
                gameState = GameState::FDG;
                audioManager.playEndMusic(finDeJeu.victoire);
                window.close();
                window.create(sf::VideoMode(900, 600), "Ramio - Fin de niveau");
            }

            
            // Gestion des vies et des morts
            int livesRemaining = 5 - (nbMortsMario + nbMortsLuigi);
            if (ramio.isDead)
            {
                ramio.respawn();
                nbMortsMario++;
                std::cout << "Vies restantes: " << livesRemaining << std::endl;
            }
            if (guili.isDead)
            {
                guili.respawn();
                nbMortsLuigi++;
                std::cout << "Vies restantes: " << livesRemaining << std::endl;
            }
        
            // Fin de jeu par défaite
            if (livesRemaining==-1)
            {
                gameState = GameState::FDG;
                window.close();
                window.create(sf::VideoMode(900, 600), "Ramio - Fin de niveau");
            }

            // Suivi de Ramio et Guili par la caméra
            sf::Vector2f posMario = ramio.getPosition();
            sf::Vector2f posLuigi = multijoueur ? guili.getPosition() : sf::Vector2f(0, 0);
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
                level.update(deltaTime, window, ramio.getGlobalBounds(), guili.getGlobalBounds());
            else
                level.update(deltaTime, window, ramio.getGlobalBounds(), sf::FloatRect());

            level.draw(window);
            ramio.draw(window);
            if (multijoueur)
                guili.draw(window);

            window.display();
            window.clear();
        }
        else if (gameState == GameState::FDG)
        {
            finDeJeu.afficher(window, totalTime, Piece::getNbPiece(), nbMortsMario + nbMortsLuigi);
            window.display();
        }
    }
    
    return 0;
}
