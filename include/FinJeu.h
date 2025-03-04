
#ifndef FINJEU_H
#define FINJEU_H

#include <SFML/Graphics.hpp>
#include <string>

class FinDeJeu {
public:
    FinDeJeu(float windowWidth, float windowHeight);
    //void afficher(sf::RenderWindow& window, float tempsEcoule, int score);
    void afficher(sf::RenderWindow& window, float tempsEcoule, int score, int nbMorts);

    bool handleInput(sf::Event event);
    bool victoire=false;

private:
    sf::Font font;
    sf::Text titre;
    sf::Text texteTemps;
    sf::Text texteScore;
    sf::Text texteInstructions;
    sf::Text texteMorts; 
    sf::RectangleShape background;
    sf::Texture backgroundTexture; // Ajout de la texture pour le fond d'écran
    sf::Sprite backgroundSprite;   // Ajout du sprite pour le fond d'écran


    sf::RectangleShape bouton; // Ajout du bouton
    sf::Text boutonTexte;      // Texte du bouton
    float windowWidth;
    float windowHeight;

    void setupText(sf::Text& text, const std::string& content, float x, float y, unsigned int size);
};

#endif 