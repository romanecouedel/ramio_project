// fin_de_jeu.hpp
#ifndef FIN_DE_JEU_HPP
#define FIN_DE_JEU_HPP

#include <SFML/Graphics.hpp>
#include <string>

class FinDeJeu {
public:
    FinDeJeu(float windowWidth, float windowHeight);
    void afficher(sf::RenderWindow& window, float tempsEcoule, int score);
    bool handleInput(const sf::Event& event);

private:
    sf::Font font;
    sf::Text titre;
    sf::Text texteTemps;
    sf::Text texteScore;
    sf::Text texteInstructions;
    sf::RectangleShape background;

    void setupText(sf::Text& text, const std::string& content, float x, float y, unsigned int size);
};

#endif // FIN_DE_JEU_HPP
