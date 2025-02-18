// fin_de_jeu.cpp
#include "FinJeu.h"
#include <iostream>

FinDeJeu::FinDeJeu(float windowWidth, float windowHeight) {
    if (!font.loadFromFile("../resources/font.ttf")) {  // Assure-toi que le chemin vers la police est correct
        std::cerr << "Erreur de chargement de la police" << std::endl;
    }

    // Configuration du fond de l'écran de fin
    background.setSize(sf::Vector2f(windowWidth, windowHeight));
    background.setFillColor(sf::Color(0, 0, 0, 150));  // Fond semi-transparent

    // Titre du jeu
    setupText(titre, "Fin du Jeu", windowWidth / 2, windowHeight / 4, 50);
    titre.setStyle(sf::Text::Bold);

    // Texte pour afficher le temps écoulé
    texteTemps.setCharacterSize(30);
    texteTemps.setPosition(windowWidth / 2, windowHeight / 2 - 40);
    texteTemps.setFillColor(sf::Color::White);

    // Texte pour afficher le score
    texteScore.setCharacterSize(30);
    texteScore.setPosition(windowWidth / 2, windowHeight / 2);
    texteScore.setFillColor(sf::Color::White);

    // Instructions pour quitter ou recommencer
    texteInstructions.setCharacterSize(20);
    texteInstructions.setPosition(windowWidth / 2, windowHeight / 2 + 60);
    texteInstructions.setFillColor(sf::Color::White);
    texteInstructions.setString("Appuyez sur 'Q' pour quitter ou 'R' pour recommencer.");
}

void FinDeJeu::setupText(sf::Text& text, const std::string& content, float x, float y, unsigned int size) {
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setPosition(x - text.getLocalBounds().width / 2, y - text.getLocalBounds().height / 2);
    text.setFillColor(sf::Color::White);
}

void FinDeJeu::afficher(sf::RenderWindow& window, float tempsEcoule, int score) {
    // Afficher le fond
    window.draw(background);

    // Afficher le titre
    window.draw(titre);

    // Afficher le temps écoulé
    texteTemps.setString("Temps écoulé: " + std::to_string(static_cast<int>(tempsEcoule)) + " secondes");
    window.draw(texteTemps);

    // Afficher le score
    texteScore.setString("Score: " + std::to_string(score));
    window.draw(texteScore);

    // Afficher les instructions
    window.draw(texteInstructions);
}

bool FinDeJeu::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Q) {
            return true; // Quitter le jeu
        } else if (event.key.code == sf::Keyboard::R) {
            return false; // Recommencer le jeu
        }
    }
    return false;
}
