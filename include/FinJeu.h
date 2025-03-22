
#ifndef FINJEU_H
#define FINJEU_H

#include <SFML/Graphics.hpp>
#include <string>

class FinDeJeu {
public:
    // constructeur
    FinDeJeu(float windowWidth, float windowHeight);

    // Méthode pour afficher l'écran de fin de jeu
    void afficher(sf::RenderWindow& window, float tempsEcoule, int score, int nbMorts);

    // Méthode pour gérer les entrées de l'utilisateur
    bool handleInput(sf::Event event); // retourne true si le bouton pour retourner au menu est cliqué
    

    bool victoire=false;

private:

//================================================= Variables =================================================
    sf::Font font;
    sf::Text titre;
    sf::Text texteTemps;
    sf::Text texteScore;
    sf::Text texteInstructions;
    sf::Text texteMorts; 
    sf::RectangleShape background;
    sf::Texture backgroundTexture; 
    sf::Sprite backgroundSprite;   


    sf::RectangleShape bouton;
    sf::Text boutonTexte;      
    float windowWidth;
    float windowHeight;

    //====================================Méthodes====================================
    void setupText(sf::Text& text, const std::string& content, float x, float y, unsigned int size);
};

#endif 