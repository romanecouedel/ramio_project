#include "FinJeu.h"
#include <iostream>

FinDeJeu::FinDeJeu(float windowWidth, float windowHeight) {
    if (!font.loadFromFile("../fonts/arial.ttf")) {  // Assure-toi que le chemin vers la police est correct
        std::cerr << "Erreur de chargement de la police" << std::endl;
    }

    // Charger la texture du fond d'écran
    if (!backgroundTexture.loadFromFile("../img/endg.png")) {
        std::cerr << "Erreur de chargement de l'image de fond" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        windowWidth / backgroundSprite.getLocalBounds().width,
        windowHeight / backgroundSprite.getLocalBounds().height
    );

    // Configuration du fond de l'écran de fin
    background.setSize(sf::Vector2f(windowWidth, windowHeight));

    // Titre du jeu
    setupText(titre, "Fin du Jeu", windowWidth / 2, windowHeight / 4, 50);
    titre.setStyle(sf::Text::Bold);
    // Configuration du bouton
    bouton.setSize(sf::Vector2f(200, 50));
    bouton.setFillColor(sf::Color::Blue);
    bouton.setPosition(windowWidth / 2 - 100, windowHeight / 2 -25 ); 

    // Texte du bouton
    setupText(boutonTexte, "Retour au Menu", windowWidth / 2, windowHeight / 2 , 20); 
}

void FinDeJeu::setupText(sf::Text& text, const std::string& content, float x, float y, unsigned int size) {
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setPosition(x - text.getLocalBounds().width / 2, y - text.getLocalBounds().height / 2);
    text.setFillColor(sf::Color::White);
}

void FinDeJeu::afficher(sf::RenderWindow& window, float tempsEcoule, int score) {
    // Afficher le fond d'écran
    window.draw(backgroundSprite);

    // Afficher le titre
    window.draw(titre);

    // Afficher le bouton
    window.draw(bouton);
    window.draw(boutonTexte);
}

bool FinDeJeu::handleInput(const sf::Event& event) {
    bool retour_menu = false;
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            retour_menu = true;
        }
    }
    return retour_menu;
}