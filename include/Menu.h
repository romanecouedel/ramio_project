#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(float width, float height);  
    void handleInput(sf::Event event); 
    void draw(sf::RenderWindow &window); // Affichage
    int getSelectedIndex() const; 
    void moveUp();
    void moveDown();

private:
    sf::Font font;
    sf::Text menuOptions[2]; // Exemple : "Jouer", "Quitter"
    sf::RectangleShape menuBoxes[2];
    int selectedIndex;
    
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    bool isSubMenu; // Indique si on est dans le sous-menu
    sf::Text subMenuOptions[4]; // Sous-menu : "1 Joueur", "2 Joueurs", "3 Joueurs", "Retour"
    sf::RectangleShape subMenuBoxes[4]; // Encadrés du sous-menu
    int subMenuIndex;
};

#endif
