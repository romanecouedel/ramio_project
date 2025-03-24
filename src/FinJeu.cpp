#include "FinJeu.h"
#include <iostream>

/**
 * @brief Constructeur de la classe FinDeJeu
 * @param windowWidth Largeur de la fenêtre
 * @param windowHeight Hauteur de la fenêtre
 */
FinDeJeu::FinDeJeu(float windowWidth, float windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight)
{   
    // Charger la police
    if (!font.loadFromFile("../fonts/arial.ttf"))
    { 
        std::cerr << "Erreur de chargement de la police" << std::endl;
    }

    // Charger la texture du fond d'écran
    if (!backgroundTexture.loadFromFile("../img/endg.png"))
    {
        std::cerr << "Erreur de chargement de l'image de fond" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        windowWidth / backgroundSprite.getLocalBounds().width,
        windowHeight / backgroundSprite.getLocalBounds().height);

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
    bouton.setPosition(windowWidth / 2 - 100, windowHeight / 2 - 25);

    // Texte du bouton
    setupText(boutonTexte, "Retour au Menu", windowWidth / 2, windowHeight / 2, 20);
}

/**
 * @brief Configure un texte SFML avec une police, une position et une taille
 * @param text Référence vers l'objet texte à configurer
 * @param content Contenu du texte
 * @param x Position en X
 * @param y Position en Y
 * @param size Taille de la police
 */
void FinDeJeu::setupText(sf::Text &text, const std::string &content, float x, float y, unsigned int size)
{
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setPosition(x - text.getLocalBounds().width / 2, y - text.getLocalBounds().height / 2);
    text.setFillColor(sf::Color::White);
}

/**
 * @brief Affiche l'écran de fin de jeu avec le score et le temps écoulé
 * @param window Fenêtre SFML où afficher l'écran de fin
 * @param tempsEcoule Temps total de la partie en secondes
 * @param score Nombre de pièces collectées
 * @param nbMorts Nombre de morts du joueur
 */
void FinDeJeu::afficher(sf::RenderWindow& window, float tempsEcoule, int score, int nbMorts)
{
    // Afficher le fond d'écran
    window.draw(backgroundSprite);

    // Configuration du titre
    std::string titreText = victoire ? "Victoire !" : "Perdu...";
    setupText(titre, titreText, windowWidth / 2, windowHeight / 3, 75);
    titre.setStyle(sf::Text::Bold);
    titre.setFillColor(victoire ? sf::Color::Green : sf::Color::Red); // Vert pour victoire, rouge pour défaite

    // Afficher le titre
    window.draw(titre);

    // Configuration score
    std::string scoreText = "Pieces: " + std::to_string(score);
    setupText(texteScore, scoreText, windowWidth / 9, windowHeight / 12, 50);
    titre.setStyle(sf::Text::Bold);
    titre.setFillColor(sf::Color::White);

    // afficher le score
    window.draw(texteScore);

    // Configuration du temps
    // Le temps écoulé est en secondes 
    int minutes = static_cast<int>(tempsEcoule) / 60;  // Diviser pour obtenir les minutes
    int secondes = static_cast<int>(tempsEcoule) % 60;  // Récupérer le reste des secondes

    // Formater le texte à afficher
    std::string timeText = "Temps: " + std::string(minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + 
                        std::string(secondes < 10 ? "0" : "") + std::to_string(secondes);

    // Appliquer ce texte à votre objet texte
    setupText(texteTemps, timeText, windowWidth / 1.25, windowHeight / 12, 50);
    texteTemps.setStyle(sf::Text::Bold);
    texteTemps.setFillColor(sf::Color::White);

    // Configuration du compteur de morts
    std::string mortsText = "Morts: " + std::to_string(nbMorts);
    setupText(texteMorts, mortsText, windowWidth / 2, windowHeight / 1.5, 50);
    texteMorts.setStyle(sf::Text::Bold);
    texteMorts.setFillColor(sf::Color::White);

    // Afficher le compteur de morts
    window.draw(texteMorts);

    // afficher le temps
    window.draw(texteTemps);

    // Afficher le bouton
    window.draw(bouton);
    window.draw(boutonTexte);
}

/**
 * @brief Gère l'entrée utilisateur pour détecter l'appui sur la touche Entrée
 * @param event Événement SFML capturé
 * @return true si la touche Entrée est pressée, false sinon
 */
bool FinDeJeu::handleInput(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        return true;
    }
    return false;
}
