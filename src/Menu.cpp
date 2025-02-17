#include "Menu.h"
#include <iostream>

Menu::Menu(float width, float height) : isSubMenu(false), selectedIndex(0), subMenuIndex(0)
{
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
    {
        std::cerr << "Erreur chargement police" << std::endl;
    }

    if (!backgroundTexture.loadFromFile("/home/alberic/Documents/Cours/Info_Object/Projet_mario/img/menu_background.png"))
    {
        std::cerr << "Erreur chargement du fond" << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        width / backgroundSprite.getLocalBounds().width,
        height / backgroundSprite.getLocalBounds().height);

    std::string options[] = {"Jouer", "Quitter"};

    for (int i = 0; i < 2; i++)
    {
        // Configuration du texte
        menuOptions[i].setFont(font);
        menuOptions[i].setString(options[i]);
        menuOptions[i].setCharacterSize(30);
        sf::FloatRect textBounds = menuOptions[i].getLocalBounds();
        menuOptions[0].setFillColor(sf::Color::Red); // "Jouer" en rouge de base
       

        // Taille et position des rectangles
        float rectWidth = textBounds.width + 40;
        float rectHeight = textBounds.height + 30;
        float xPos = width / 2 - rectWidth / 2;
        float yPos = height / 2 - 50 + i * 100;

        menuBoxes[i].setSize(sf::Vector2f(rectWidth, rectHeight));
        menuBoxes[i].setPosition(xPos, yPos);
        menuBoxes[i].setFillColor(i == 0 ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50)); // Gris clair pour l'actif
        menuBoxes[i].setOutlineThickness(3);
        menuBoxes[i].setOutlineColor(sf::Color::White);

        // Centrer le texte
        menuOptions[i].setPosition(xPos + (rectWidth - textBounds.width) / 2, yPos + (rectHeight - textBounds.height) / 2 - 5);
        menuOptions[i].setFillColor(sf::Color::White);
    }

    std::string subOptions[] = {"1 Joueur", "2 Joueurs", "3 Joueurs", "Retour"};
    float rectWidth = 200; // Taille uniforme des rectangles
    float rectHeight = 40; // Hauteur uniforme

    for (int i = 0; i < 4; i++)
    {
        // Configuration du texte
        subMenuOptions[i].setFont(font);
        subMenuOptions[i].setString(subOptions[i]);
        subMenuOptions[i].setCharacterSize(25); // Police plus petite
        sf::FloatRect textBounds = subMenuOptions[i].getLocalBounds();

        // Centrage horizontal et position verticale
        float xPos = width / 2 - rectWidth / 2;
        float yPos = height / 2 - 100 + i * 80;

        // Configuration des rectangles
        subMenuBoxes[i].setSize(sf::Vector2f(rectWidth, rectHeight));
        subMenuBoxes[i].setPosition(xPos, yPos);
        subMenuBoxes[i].setFillColor(sf::Color(50, 50, 50)); // Fond normal en gris foncé
        subMenuBoxes[i].setOutlineThickness(3);
        subMenuBoxes[i].setOutlineColor(sf::Color::White);

        // Centrage du texte
        subMenuOptions[i].setPosition(
            xPos + (rectWidth - textBounds.width) / 2,
            yPos + (rectHeight - textBounds.height) / 2 - 5);
        subMenuOptions[i].setFillColor(i == 0 ? sf::Color::Red : sf::Color::White); // Option par défaut en rouge
    }
}

void Menu::moveUp()
{
    if (selectedIndex > 0)
    {
        menuBoxes[selectedIndex].setFillColor(sf::Color(50, 50, 50)); // Fond normal
        selectedIndex--;
        menuBoxes[selectedIndex].setFillColor(sf::Color(100, 100, 100)); // Met en surbrillance
    }
}

void Menu::moveDown()
{
    if (selectedIndex < 1)
    {
        menuBoxes[selectedIndex].setFillColor(sf::Color(50, 50, 50));
        selectedIndex++;
        menuBoxes[selectedIndex].setFillColor(sf::Color(100, 100, 100));
    }
}

int Menu::getSelectedIndex() const
{
    return selectedIndex;
}

void Menu::handleInput(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (isSubMenu)
        {
            if (event.key.code == sf::Keyboard::Up)
            {
                if (subMenuIndex > 0)
                {
                    subMenuOptions[subMenuIndex].setFillColor(sf::Color::White); // Remettre en blanc
                    subMenuIndex--;
                    subMenuOptions[subMenuIndex].setFillColor(sf::Color::Red); // Rouge pour l'option active
                }
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                if (subMenuIndex < 3)
                {
                    subMenuOptions[subMenuIndex].setFillColor(sf::Color::White);
                    subMenuIndex++;
                    subMenuOptions[subMenuIndex].setFillColor(sf::Color::Red);
                }
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                if (subMenuIndex == 3)
                {
                    isSubMenu = false; // Retour au menu principal
                }
                else
                {
                    std::cout << "Lancement du jeu avec " << (subMenuIndex + 1) << " joueur(s) !" << std::endl;
                }
            }
        }
        else
        {
            if (event.key.code == sf::Keyboard::Up)
            {
                if (selectedIndex > 0)
                {
                    menuOptions[selectedIndex].setFillColor(sf::Color::White);
                    selectedIndex--;
                    menuOptions[selectedIndex].setFillColor(sf::Color::Red);
                }
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                if (selectedIndex < 1)
                {
                    menuOptions[selectedIndex].setFillColor(sf::Color::White);
                    selectedIndex++;
                    menuOptions[selectedIndex].setFillColor(sf::Color::Red);
                }
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                if (selectedIndex == 0)
                {
                    isSubMenu = true; // Passage au sous-menu
                    for (int i = 0; i < 4; i++)
                    {
                        subMenuOptions[i].setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
                    }

                    subMenuIndex = 0;
                }
                else if (selectedIndex == 1)
                {
                    exit(0); // Ferme le programme
                }
            }
        }
    }
}

void Menu::draw(sf::RenderWindow &window)
{
    window.draw(backgroundSprite);

    if (isSubMenu)
    {
        for (int i = 0; i < 4; i++)
        {
            window.draw(subMenuBoxes[i]);
            window.draw(subMenuOptions[i]);
        }
    }
    else
    {
        for (int i = 0; i < 2; i++)
        {
            window.draw(menuBoxes[i]);
            window.draw(menuOptions[i]);
        }
    }
}
