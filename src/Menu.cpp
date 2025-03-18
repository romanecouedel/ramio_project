#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Menu.h"

// Constructeur
Menu::Menu(float width, float height) : currentState(MenuState::MAIN), selectedIndex(0)
{
    // Chargement de la police
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Erreur chargement police" << std::endl;
    }
    // Chargement de l'arrière-plan + dimensionnement
    if (!backgroundTexture.loadFromFile("../img/menu_background.png"))
    {
        std::cerr << "Erreur chargement du fond" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(width / backgroundSprite.getLocalBounds().width, height / backgroundSprite.getLocalBounds().height);

    // Initialisation des options des différents menus
    setupMenu(mainMenu, {"Jouer", "Quitter"}, width, height);
    setupMenu(playerMenu, {"1 Joueur", "2 Joueurs", "Retour"}, width, height);
    setupMenu(aiMenu, {"En local", "Avec l'ordinateur", "Retour"}, width, height);
    setupMenu(levelMenu, {"Niveau 1", "Niveau 2", "Niveau 3", "Retour"}, width, height);
}

// Configurations des différents menus
void Menu::setupMenu(std::vector<MenuOption> &menu, const std::vector<std::string> &options, float width, float height)
{
    menu.clear();
    for (size_t i = 0; i < options.size(); i++)
    {
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

// Gestion des entrées clavier
void Menu::handleInput(sf::Event event, sf::RenderWindow &window, bool &luigiAI, bool &multijoueur)
{
    std::vector<MenuOption> *currentMenu = nullptr;

    switch (currentState)
    {
    case MenuState::MAIN:
        currentMenu = &mainMenu;
        break;
    case MenuState::PLAYER_SELECT:
        currentMenu = &playerMenu;
        break;
    case MenuState::MODE_SELECT:
        currentMenu = &aiMenu;
        break;
    case MenuState::LEVEL_SELECT:
        currentMenu = &levelMenu;
        break;
    default:
        return;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Up)
            changeSelection(*currentMenu, -1);
        else if (event.key.code == sf::Keyboard::Down)
            changeSelection(*currentMenu, 1);
        else if (event.key.code == sf::Keyboard::Enter)
        {
            int choice = selectedIndex; // Récupération du choix d'option fait par le joueur

            switch (currentState) // changement de l'état du menu en fonction du choix et du l'état actuel
            {
            case MenuState::MAIN:
                if (choice == 0)
                {
                    setState(MenuState::PLAYER_SELECT);
                }
                else if (choice == 1)
                {
                    window.close();
                }
                break;

            case MenuState::PLAYER_SELECT:
                if (choice == 0)
                {
                    setState(MenuState::LEVEL_SELECT);
                    multijoueur = false;
                }
                else if (choice == 1)
                {
                    setState(MenuState::MODE_SELECT);
                    multijoueur = true;
                }
                else
                {
                    setState(MenuState::MAIN);
                }
                break;

            case MenuState::MODE_SELECT:
                if (choice == 0)
                {
                    std::cout << "Mode 2 joueurs en local sélectionné !" << std::endl;
                    setState(MenuState::LEVEL_SELECT);
                    multijoueur = true;
                    luigiAI = false;
                }
                else if (choice == 1)
                {
                    std::cout << "Mode contre l'IA sélectionné !" << std::endl;
                    setState(MenuState::LEVEL_SELECT);
                    multijoueur = true;
                    luigiAI = true;
                }
                else
                {
                    setState(MenuState::PLAYER_SELECT);
                }
                break;

            case MenuState::LEVEL_SELECT:
                if (choice == 3)
                {
                    setState(MenuState::PLAYER_SELECT);
                }
                else
                {
                    std::cout << "Lancement du niveau " << choice + 1 << " !" << std::endl;
                    setState(MenuState::GAMEOK);
                }
                break;

            default:
                break;
            }
        }
    }
}

// affichage du menu
void Menu::draw(sf::RenderWindow &window)
{
    // affichage de l'arrière-plan
    window.draw(backgroundSprite);

    std::vector<MenuOption> *currentMenu = nullptr;

    // affichage des options du menu en fonction de l'état actuel
    switch (currentState)
    {
    case MenuState::MAIN:
        currentMenu = &mainMenu;
        break;
    case MenuState::PLAYER_SELECT:
        currentMenu = &playerMenu;
        break;
    case MenuState::MODE_SELECT:
        currentMenu = &aiMenu;
        break;
    case MenuState::LEVEL_SELECT:
        currentMenu = &levelMenu;
        break;
    default:
        return;
    }

    // affichage des options du menu
    for (auto &option : *currentMenu)
    {
        window.draw(option.box);
        window.draw(option.text);
    }
}

// Changement de l'option sélectionnée, utilisé dans handleInput
void Menu::changeSelection(std::vector<MenuOption> &menu, int direction)
{
    selectedIndex = (selectedIndex + direction + menu.size()) % menu.size();
    updateMenuColors(menu);
}

void Menu::updateMenuColors(std::vector<MenuOption> &menu)
{
    for (size_t i = 0; i < menu.size(); i++)
    {
        menu[i].text.setFillColor(i == selectedIndex ? sf::Color::Red : sf::Color::White);
    }
}

// Vérification si le jeu est prêt à être lancé, utilisé dans main
bool Menu::isGameStarting() const
{
    return currentState == MenuState::GAMEOK && selectedIndex != 3;
}

// Changement de l'état du menu, utilisé dans handleInput
void Menu::setState(MenuState newState)
{
    currentState = newState;
    selectedIndex = 0;
}

// Réinitialisation du menu, utilisé dans main
void Menu::reset()
{
    currentState = MenuState::MAIN;
    selectedIndex = 0;
    // réinitialisation des sélections visuelles des options
    updateMenuColors(mainMenu);
    updateMenuColors(playerMenu);
    updateMenuColors(aiMenu);
    updateMenuColors(levelMenu);
}
