#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Menu.h"


Menu::Menu(float width, float height) : currentState(MenuState::MAIN), selectedIndex(0) {
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Erreur chargement police" << std::endl;
    }
    if (!backgroundTexture.loadFromFile("../img/menu_background.png")) {
        std::cerr << "Erreur chargement du fond" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(width / backgroundSprite.getLocalBounds().width, height / backgroundSprite.getLocalBounds().height);
    
    setupMenu(mainMenu, {"Jouer", "Quitter"}, width, height);
    setupMenu(playerMenu, {"1 Joueur", "2 Joueurs", "Retour"}, width, height);
    setupMenu(local_ia_menu, {"En local", "Avec l'ordinateur", "Retour"}, width, height);
    setupMenu(levelMenu, {"Niveau 1", "Niveau 2", "Niveau 3", "Retour"}, width, height);
}

void Menu::setupMenu(std::vector<MenuOption> &menu, std::vector<std::string> options, float width, float height) {
    menu.clear();
    for (size_t i = 0; i < options.size(); i++) {
        MenuOption option;
        option.text.setFont(font);
        option.text.setString(options[i]);
        option.text.setCharacterSize(30);
        option.text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        
        sf::FloatRect textBounds = option.text.getLocalBounds();
        float rectWidth = textBounds.width + 40;
        float rectHeight = textBounds.height + 30;
        float xPos = width / 2 - rectWidth / 2;
        float yPos = height / 2 - 100 + i * 80;
        
        option.box.setSize(sf::Vector2f(rectWidth, rectHeight));
        option.box.setPosition(xPos, yPos);
        option.box.setFillColor(sf::Color(50, 50, 50));
        option.box.setOutlineThickness(3);
        option.box.setOutlineColor(sf::Color::White);
        
        option.text.setPosition(xPos + (rectWidth - textBounds.width) / 2, yPos + (rectHeight - textBounds.height) / 2 - 5);
        
        menu.push_back(option);
    }
}


void Menu::handleInput(sf::Event event, sf::RenderWindow &window) {
    std::vector<MenuOption> *currentMenu;
    if (currentState == MenuState::MAIN) currentMenu = &mainMenu;
    else if (currentState == MenuState::PLAYER_SELECT) currentMenu = &playerMenu;
    else if (currentState == MenuState::MODE_SELECT) currentMenu = &local_ia_menu;
    else currentMenu = &levelMenu;

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) changeSelection(*currentMenu, -1);
        else if (event.key.code == sf::Keyboard::Down) changeSelection(*currentMenu, 1);
        else if (event.key.code == sf::Keyboard::Enter) {
            int choice = selectedIndex;

            if (currentState == MenuState::MAIN) {
                if (choice == 0) {
                    currentState = MenuState::PLAYER_SELECT;
                    selectedIndex = 0; // Reset selection
                }
                else if (choice == 1) window.close();
            } 
            else if (currentState == MenuState::PLAYER_SELECT) {
                if (choice == 0) {
                    currentState = MenuState::LEVEL_SELECT; // 1 joueur → niveau
                    selectedIndex = 0; // Reset selection
                }
                else if (choice == 1) {
                    currentState = MenuState::MODE_SELECT; // 2 joueurs → choix IA/local
                    selectedIndex = 0; // Reset selection
                }
                else if (choice == 2) {
                    currentState = MenuState::MAIN; // Retour
                    selectedIndex = 0; // Reset selection
                }
            } 
            else if (currentState == MenuState::MODE_SELECT) {
                if (choice == 0) {
                    std::cout << "Mode 2 joueurs en local sélectionné !" << std::endl;
                    currentState = MenuState::LEVEL_SELECT;
                    selectedIndex = 0; // Reset selection
                } else if (choice == 1) {
                    std::cout << "Mode contre l'IA sélectionné !" << std::endl;
                    currentState = MenuState::LEVEL_SELECT;
                    selectedIndex = 0; // Reset selection
                } else if (choice == 2) {
                    currentState = MenuState::PLAYER_SELECT;
                    selectedIndex = 0; // Reset selection
                }
            }
            else if (currentState == MenuState::LEVEL_SELECT) {
                if (choice == 3) {
                    currentState = MenuState::PLAYER_SELECT;
                    selectedIndex = 0; // Reset selection
                }
                else{
                    std::cout << "Lancement du niveau " << choice + 1 << " !" << std::endl;
                    currentState = MenuState::GAMEOK;
                }
            }
        }
    }
}

void Menu::draw(sf::RenderWindow &window) {
    window.draw(backgroundSprite);
    std::vector<MenuOption> *currentMenu = 
        (currentState == MenuState::MAIN) ? &mainMenu :
        (currentState == MenuState::PLAYER_SELECT) ? &playerMenu :
        (currentState == MenuState::MODE_SELECT) ? &local_ia_menu :
        &levelMenu;

    for (auto &option : *currentMenu) {
        window.draw(option.box);
        window.draw(option.text);
    }
}

void Menu::changeSelection(std::vector<MenuOption> &menu, int direction) {
    menu[selectedIndex].text.setFillColor(sf::Color::White);
    selectedIndex = (selectedIndex + direction + menu.size()) % menu.size();
    menu[selectedIndex].text.setFillColor(sf::Color::Red);
}

bool Menu::isGameStarting() {
    return currentState == MenuState::GAMEOK && selectedIndex != 3; // Si un niveau est sélectionné //3=nb niveau
}
