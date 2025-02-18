#include "Level.h"
#include "Confetti.h"
#include "Bloc.h"
#include "Player.h"
#include <fstream>
#include <iostream>

Level::Level() {}

// ======================== Chargement du Niveau ========================
bool Level::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Erreur chargement niveau : " << filename << std::endl;
        return false;
    }

    grid.clear();
    blocs.clear();

    float blockSize = 32.0f;
    std::string line;
    int y = 0;

    while (std::getline(file, line))
    {
        std::vector<int> row;
        for (int x = 0; x < static_cast<int>(line.size()); ++x)
        {
            char c = line[x];
            row.push_back(c);

            sf::Vector2f position(x * blockSize, y * blockSize);

            if (c == '#')
            {
                auto bloc = std::make_unique<BlocSol>();
                bloc->setPosition(position.x, position.y);
                blocs.push_back(std::move(bloc));
            }
            else if (c == '?')
            {
                auto bloc = std::make_unique<BlocMystere>();
                bloc->setPosition(position.x, position.y);
                blocs.push_back(std::move(bloc));
            }
            else if (c == '!')
            {
                drapeau.setPosition(position.x, position.y);
            }
        }
        grid.push_back(row);
        ++y;
    }
    return true;
}

// ======================== Affichage du Niveau ========================
void Level::draw(sf::RenderWindow &window)
{
    for (const auto &bloc : blocs)
    {
        bloc->draw(window);
    }
    drapeau.draw(window);

    if (afficherTexte)
    {
        window.draw(niveauTermineText);
    }
}

void Level::startConfetti()
{
    confettis.clear();
    confettiStack.clear();

    // Création massive de confettis sur toute la largeur
    for (int i = 0; i < 500; ++i)
    {
        float x = static_cast<float>(std::rand() % 1280);
        float y = static_cast<float>(std::rand() % 200 - 200); // Apparaît au-dessus
        float speed = static_cast<float>(100 + std::rand() % 100);
        confettis.emplace_back(x, y, speed);
    }
    confettiActive = true;
}

void Level::update(float deltaTime, sf::RenderWindow &window, const sf::FloatRect &playerHitbox)
{
    // Animation de confettis (déjà existante)
    if (confettiActive)
    {
        for (auto &confetti : confettis)
        {
            confetti.update(deltaTime);
        }
    }
    for (auto &bloc : blocs)
    {
        auto *blocMystere = dynamic_cast<BlocMystere *>(bloc.get());
        if (blocMystere)
        {
            blocMystere->update(deltaTime, window);

            // Hitbox avec tolérance
            sf::FloatRect hitboxAvecTolerance = blocMystere->getGlobalBounds();
            hitboxAvecTolerance.top -= 3.0f;
            hitboxAvecTolerance.height += 6.0f;

            // Vérifie si le joueur frappe par en dessous uniquement
            if (playerHitbox.intersects(hitboxAvecTolerance))
            {
                // Détecte si le joueur frappe par en dessous (player au-dessous du bloc)
                float milieuBloc = blocMystere->getGlobalBounds().top + blocMystere->getGlobalBounds().height * 0.8f;
                if (!blocMystere->isAnimating() && playerHitbox.top > milieuBloc)
                {
                    blocMystere->onHit();
                }
            }
        }
    }
    // Animation de zoom avec centrage dynamique
    if (afficherTexte)
    {
        // Animation de zoom
        float zoomSpeed = 0.5f; // Vitesse de zoom
        float epsilon = 0.01f;  // Tolérance pour éviter les blocages

        // Animation de zoom aller-retour
        if (zoomIn)
        {
            zoomScale += zoomSpeed * deltaTime;
            if (zoomScale >= 1.5f - epsilon)
            {
                zoomScale = 1.5f; // Correction exacte
                zoomIn = false;   // Passer au zoom arrière
            }
        }
        else
        {
            zoomScale -= zoomSpeed * deltaTime;
            if (zoomScale <= 1.0f + epsilon)
            {
                zoomScale = 1.0f; // Correction exacte
                zoomIn = true;    // Repasser au zoom avant
            }
        }

        // Centrage dynamique (garder le texte centré malgré le zoom)
        sf::FloatRect bounds = niveauTermineText.getLocalBounds();
        niveauTermineText.setOrigin(bounds.width / 2, bounds.height / 2);
        niveauTermineText.setPosition(window.getSize().x / 2, window.getSize().y / 2);
        niveauTermineText.setScale(zoomScale, zoomScale);
    }
}

// ======================== Détection de Collisions ========================
bool Level::isColliding(const sf::FloatRect &hitbox) const
{
    for (const auto &bloc : blocs)
    {
        if (hitbox.intersects(bloc->getGlobalBounds()))
        {
            return true;
        }
    }
    return false;
}

void Level::initTexte()
{
    if (!font.loadFromFile("../fonts/arial.ttf"))
    {
        std::cerr << "Erreur chargement police!" << std::endl;
    }
    niveauTermineText.setFont(font);
    niveauTermineText.setString("Niveau Termine");
    niveauTermineText.setCharacterSize(70);
    niveauTermineText.setFillColor(sf::Color::Yellow);
    niveauTermineText.setOutlineColor(sf::Color::Red);
    niveauTermineText.setOutlineThickness(4);
}
