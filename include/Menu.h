#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// ================ États possibles du menu ==================
enum class MenuState {
    MAIN,               // Menu accueil
    PLAYER_SELECT,      // Sélection du nombre de joueurs
    MODE_SELECT,        // Sélection ia ou local
    LEVEL_SELECT,       // Sélection du niveau
    GAMEOK              // Lancement du jeu
};

// Classe représentant l'affichage, l'interaction et la logique du menu

class Menu {
public:
    // Constructeur
    Menu(float width, float height);
    ~Menu() {
        std::cout << "Destruction de Menu" << std::endl;
    }
    // Interaction menu/joueur
    void handleInput(sf::Event event, sf::RenderWindow &window, bool &luigiAI, bool &multijoueur); // est appelé par le main

    // Affichage du menu
    void draw(sf::RenderWindow &window); //est appelé par le main
    
    // Vérifie si le jeu est prêt à etre lancé true=prêt false sinon
    bool isGameStarting() const; //récupérer par le main pour lancer le jeu

    // Methode pour récupérer le niveau sélectionné, retourne le numéro du level
    int getSelectedLevel() const { return selectedIndex; } //récupérer par le main pour lancer le niveau sélectionné par le joueur

    void reset(); // Réinitialiser le menu depuis le main lorsque la partie est terminée

private:
// ======================== Variables ========================
    /**
     * @brief Structure interne représentant une option du menu.
     */
    struct MenuOption {
        sf::Text text;              // Texte de l'option du menu.
        sf::RectangleShape box;     // Boîte englobante pour l'affichage et la sélection.
    };
    
    sf::Font font;                 // Police utilisée pour le texte des menus.
    sf::Texture backgroundTexture;  // Texture de l'arrière-plan du menu.
    sf::Sprite backgroundSprite;    // Sprite affichant l'arrière-plan.

    MenuState currentState;         // État actuel du menu.
    int selectedIndex;              // Index de l'option actuellement sélectionnée.


    std::vector<MenuOption> mainMenu;    // Options du menu principal.
    std::vector<MenuOption> playerMenu;  // Options de sélection du nombre de joueurs.
    std::vector<MenuOption> levelMenu;   // Options de sélection du niveau.
    std::vector<MenuOption> aiMenu;      // Options de sélection local ou IA.

// ======================== Méthodes ========================
    // configure les options du menu passées en paramètre
    void setupMenu(std::vector<MenuOption> &menu, const std::vector<std::string> &options, float width, float height);
    
    // Change l'option sélectionnée en fonction de la touche du clavier appuyée (direction),
    void changeSelection(std::vector<MenuOption> &menu, int direction);

    // Change l'état du menu actuel par newState et remet l'index à 0
    void setState(MenuState newState); 

    void updateMenuColors(std::vector<MenuOption> &menu); // Met à jour les couleurs des options du menu.

};

#endif
