#include "FinJeu.h"
#include <iostream>

FinDeJeu::FinDeJeu(float windowWidth, float windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight) {
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

    // Configuration du titre
    std::string titreText = victoire ? "Victoire !" : "Perdu...";
    setupText(titre, titreText, windowWidth / 2, windowHeight / 4, 50);
    titre.setStyle(sf::Text::Bold);
    titre.setFillColor(victoire ? sf::Color::Green : sf::Color::Red); // Vert pour victoire, rouge pour défaite


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

    // Configuration du titre
    std::string titreText = victoire ? "Victoire !" : "Perdu...";
    setupText(titre, titreText, windowWidth / 2, windowHeight / 3, 75);
    titre.setStyle(sf::Text::Bold);
    titre.setFillColor(victoire ? sf::Color::Green : sf::Color::Red); // Vert pour victoire, rouge pour défaite

    // Afficher le titre
    window.draw(titre);

    //Configuration score
    std::string scoreText = "Pieces: " + std::to_string(score);
    setupText(texteScore, scoreText, windowWidth / 9, windowHeight / 12, 50);
    titre.setStyle(sf::Text::Bold);
    titre.setFillColor(sf::Color::White);
    
    //afficher le score
    window.draw(texteScore);

    //Configuration time
    int seconds = static_cast<int>(tempsEcoule* 60 *60) ;
    int min = static_cast<int>(seconds/60) ;
    std::string timeText = "Temps: " + std::string(min < 10 ? "0" : "") + std::to_string(min) + ":" + std::string(seconds < 10 ? "0" : "") + std::to_string(seconds);
    setupText(texteTemps, timeText, windowWidth / 1.25, windowHeight / 12, 50);
    texteTemps.setStyle(sf::Text::Bold);
    texteTemps.setFillColor(sf::Color::White);

    //afficher le temps
    window.draw(texteTemps);


    // Afficher le bouton
    window.draw(bouton);
    window.draw(boutonTexte);
}

bool FinDeJeu::handleInput(sf::Event event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        return true;
    }
    return false;
}