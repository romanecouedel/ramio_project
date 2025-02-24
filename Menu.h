#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

enum class MenuState { MAIN, PLAYER_SELECT,MODE_SELECT, LEVEL_SELECT, GAMEOK };

class Menu {
public:
    Menu(float width, float height);
    void handleInput(sf::Event event, sf::RenderWindow &window);
    void draw(sf::RenderWindow &window);
    bool isGameStarting();
    int getSelectedLevel() const { return selectedIndex; }

private:
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    MenuState currentState;
    int selectedIndex;
    
    struct MenuOption {
        sf::Text text;
        sf::RectangleShape box;
    };
    
    std::vector<MenuOption> mainMenu;
    std::vector<MenuOption> playerMenu;
    std::vector<MenuOption> levelMenu;
    std::vector<MenuOption> local_ia_menu;
    
    void setupMenu(std::vector<MenuOption> &menu, std::vector<std::string> options, float width, float height);
    int getClickedIndex(std::vector<MenuOption> &menu, sf::Vector2f mousePos);
    void changeSelection(std::vector<MenuOption> &menu, int direction);
};


#endif
