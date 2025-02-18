#include <SFML/Graphics.hpp>

class ScoreManager {
public:
    static ScoreManager& getInstance() {
        static ScoreManager instance;
        return instance;
    }

    void incrementPieceCount() {
        pieceCount++;
        updatePieceCounter();
    }

    void updatePieceCounter() {
        pieceText.setString("Pièces: " + std::to_string(pieceCount));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(pieceText);
    }

private:
    int pieceCount = 0;
    sf::Font font;
    sf::Text pieceText;

    ScoreManager() {
        if (!font.loadFromFile("assets/fonts/arial.ttf")) {
            std::cerr << "Erreur : Impossible de charger la police." << std::endl;
        }
        pieceText.setFont(font);
        pieceText.setCharacterSize(24);
        pieceText.setFillColor(sf::Color::Yellow);
        pieceText.setPosition(10, 10);
    }
};
