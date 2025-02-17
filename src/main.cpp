#include <SFML/Graphics.hpp>
#include <iostream>
#include "Level.h"
#include "Player.h"

int main() {
    Level level;
    bool niveauTermine = false;

    if (!level.loadFromFile("../levels/level2.txt")) {
        std::cerr << "Erreur chargement niveau!" << std::endl;
        return -1;
    }

    // Ajustement de la taille de la fenêtre selon le niveau
    float blockSize = 32.0f;
    unsigned int windowWidth = level.getWidth() * blockSize;
    unsigned int windowHeight = level.getHeight() * blockSize;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Mario");

    Player player;
    sf::Clock clock;
    level.initTexte();

    while (window.isOpen()) {
        sf::Event event;
        float deltaTime = clock.restart().asSeconds();
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        player.handleInput();
        player.update(deltaTime, level);
        if (player.getHitbox().intersects(level.getDrapeau().getGlobalBounds()) && niveauTermine == false) {
            std::cout << "Niveau terminé !" << std::endl;
            niveauTermine = true;
            level.startConfetti(); 
            level.afficherTexte = true;
        }
        level.update(deltaTime, window);
        
        window.clear(sf::Color::Blue);
        level.draw(window);
        player.draw(window);
        window.display();
    }
    return 0;
}