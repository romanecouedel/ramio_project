#include "Level.h"
#include "Bloc.h"
#include "Player.h"
#include "Audio.h"
#include <fstream>
#include <iostream>
#include <cmath>

extern AudioManager audioManager;
sf::Texture Level::bgTextureLeft;
sf::Texture Level::bgTextureRight;

Level::Level()
{

    if (Level::bgTextureLeft.getSize().x == 0)
    {
        Level::bgTextureLeft.loadFromFile("../img/background1.png");
    }
    if (Level::bgTextureRight.getSize().x == 0)
    {
        Level::bgTextureRight.loadFromFile("../img/background2.png");
    }

    bgWidth = bgTextureLeft.getSize().x; // Largeur d'une image

    // Créer un vertex array pour dessiner le fond
    backgroundVertices.setPrimitiveType(sf::Quads);
}

Level::~Level()
{
    blocs.clear();              // Supprime tous les blocs
    grid.clear();               // Vide la grille
    backgroundVertices.clear(); // Nettoie les vertices
}

// ======================== Chargement du Niveau ========================
bool Level::loadFromFile(const std::string &filename)
{

    blocs.clear();
    blocs.shrink_to_fit(); // Force la libération de mémoire
    grid.clear();
    grid.shrink_to_fit();
    backgroundVertices.clear();
    backgroundVertices.resize(0); // ✅ Vide proprement le VertexArray

    blocs.clear();
    grid.clear();
    backgroundVertices.clear();

    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Erreur chargement niveau : " << filename << std::endl;
        return false;
    }

    grid.clear();
    blocs.clear();

    float blockSize = 64.0f;
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
            else if (c == 'U' || c == 'V')
            {
                Tuyau::Type type = (c == 'U') ? Tuyau::Type::ENTREE : Tuyau::Type::SORTIE;
                auto tuyau = std::make_unique<Tuyau>(type);
                tuyau->setPosition(position.x, position.y);
                blocs.push_back(std::move(tuyau));
            }
        }
        grid.push_back(row);
        ++y;
    }

    if (!grid.empty())
    {
        generateBackground(grid[0].size() * 64, grid.size() * 64);
    }

    return true;
}
// ======================== Affichage du Niveau ========================
void Level::draw(sf::RenderWindow &window)
{
    // Dessiner l'arrière-plan
    sf::RenderStates statesLeft;
    statesLeft.texture = &bgTextureLeft;
    sf::RenderStates statesRight;
    statesRight.texture = &bgTextureRight;

    for (size_t i = 0; i < backgroundVertices.getVertexCount(); i += 4)
    {
        if ((i / 4) % 2 == 0)
            window.draw(&backgroundVertices[i], 4, sf::Quads, statesLeft);
        else
            window.draw(&backgroundVertices[i], 4, sf::Quads, statesRight);
    }

    // Dessiner les blocs
    bool coinSoundPlayed = false;
    for (const auto &bloc : blocs)
    {
        bloc->draw(window);

        if (auto *blocMystere = dynamic_cast<BlocMystere *>(bloc.get()))
        {
            if (blocMystere->isAnimating() && blocMystere->getPiece())
            {
                blocMystere->getPiece()->draw(window);
                if (!coinSoundPlayed)
                {
                    audioManager.playCoinSound();
                    coinSoundPlayed = true;
                }
            }
        }
    }
    drapeau.draw(window);
}

// ======================== Mise à Jour ========================
void Level::update(float deltaTime, sf::RenderWindow &window, const sf::FloatRect &hitboxMario, const sf::FloatRect &hitboxLuigi)
{
    for (auto &bloc : blocs)
    {
        auto *blocMystere = dynamic_cast<BlocMystere *>(bloc.get());
        if (blocMystere)
        {
            blocMystere->update(deltaTime, window);

            sf::FloatRect blocBounds = blocMystere->getGlobalBounds();
            sf::FloatRect hitboxAvecTolerance = blocBounds;
            hitboxAvecTolerance.top -= 3.0f;
            hitboxAvecTolerance.height += 6.0f;

            for (const auto &hitboxJoueur : {hitboxMario, hitboxLuigi})
            {
                if (hitboxJoueur.intersects(hitboxAvecTolerance))
                {
                    float blocCenterX = blocBounds.left + blocBounds.width / 2.0f;
                    float joueurCenterX = hitboxJoueur.left + hitboxJoueur.width / 2.0f;
                    float maxOffset = blocBounds.width * 0.4f;

                    if (std::abs(joueurCenterX - blocCenterX) < maxOffset)
                    {
                        float milieuBloc = blocBounds.top + blocBounds.height * 0.4f;

                        if (!blocMystere->isAnimating() && hitboxJoueur.top > milieuBloc)
                        {
                            blocMystere->onHit();
                        }
                        blocMystere->estTouche = true;
                    }
                }
            }
        }
    }
}
// ======================== Détection de Collisions Optimisée ========================
bool Level::isColliding(const sf::FloatRect &hitbox) const
{
    float blockSize = 64.0f; // Taille d'un bloc
    int startX = std::max(0, static_cast<int>(hitbox.left / blockSize));
    int startY = std::max(0, static_cast<int>(hitbox.top / blockSize));
    int endX = std::min(getWidth() - 1, static_cast<int>((hitbox.left + hitbox.width) / blockSize));
    int endY = std::min(getHeight() - 1, static_cast<int>((hitbox.top + hitbox.height) / blockSize));

    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            if (grid[y][x] == '#' || grid[y][x] == '?' || grid[y][x] == 'U' || grid[y][x] == 'V') // Blocs solides
            {
                return true;
            }
        }
    }
    return false;
}

//==========================detection bloc mystere proche==========================
BlocMystere *Level::getBlocMystereProche(const sf::Vector2f &position)
{
    float blockSize = 64.0f;  // Taille d'un bloc dans ton niveau
    float tolerance = 500.0f; // Tolérance pour la vérification des positions

    // Définition des directions autour du joueur (haut, bas, gauche, droite)
    std::vector<sf::Vector2f> directions = {
        {0, -blockSize}, // Au-dessus
        {0, blockSize},  // En dessous
        {-blockSize, 0}, // À gauche
        {blockSize, 0}   // À droite
    };

    for (const auto &dir : directions)
    {
        sf::Vector2f checkPosition = position + dir;

        for (const auto &bloc : blocs)
        {
            if (auto *blocMystere = dynamic_cast<BlocMystere *>(bloc.get()))
            {
                sf::Vector2f blocPosition = blocMystere->getPosition();
                if (std::abs(blocPosition.x - checkPosition.x) < tolerance &&
                    std::abs(blocPosition.y - checkPosition.y) < tolerance)
                {
                    // Vérifie si le bloc mystère contient encore un objet (non vide)
                    if (!blocMystere->estTouche)
                    {
                        return blocMystere;
                    }
                }
            }
        }
    }

    return nullptr; // Aucun bloc mystère valide trouvé
}

void Level::afficherEtatBlocsMysteres() const
{
    for (const auto &bloc : blocs)
    {
        if (auto *blocMystere = dynamic_cast<BlocMystere *>(bloc.get()))
        {
            std::cout << "Bloc Mystere à (" << blocMystere->getPosition().x << ", " << blocMystere->getPosition().y << ") - ";
            if (blocMystere->estTouche)
            {
                std::cout << "Touché" << std::endl;
            }
            else
            {
                std::cout << "Non touché" << std::endl;
            }
        }
    }
}

// ======================== Initialisation du Texte ========================
void Level::initTexte()
{
    if (!font.loadFromFile("../fonts/arial.ttf"))
    {
        std::cerr << "Erreur chargement police!" << std::endl;
    }
    niveauTermineText.setFont(font);
    niveauTermineText.setString("Niveau Terminé");
    niveauTermineText.setCharacterSize(70);
    niveauTermineText.setFillColor(sf::Color::Yellow);
    niveauTermineText.setOutlineColor(sf::Color::Red);
    niveauTermineText.setOutlineThickness(4);
}

// ======================== Génération du Fond ========================
void Level::generateBackground(float levelWidth, float levelHeight)
{
    backgroundVertices.clear();
    float tileSize = bgWidth; // Taille d'un motif

    for (float x = 0; x < levelWidth; x += tileSize)
    {
        sf::Vertex quad[4];

        quad[0].position = sf::Vector2f(x, 0);
        quad[1].position = sf::Vector2f(x + tileSize, 0);
        quad[2].position = sf::Vector2f(x + tileSize, levelHeight);
        quad[3].position = sf::Vector2f(x, levelHeight);

        // Alterne entre bgLeft et bgRight
        sf::Texture &texture = (static_cast<int>(x / tileSize) % 2 == 0) ? bgTextureLeft : bgTextureRight;

        quad[0].texCoords = sf::Vector2f(0, 0);
        quad[1].texCoords = sf::Vector2f(tileSize, 0);
        quad[2].texCoords = sf::Vector2f(tileSize, levelHeight);
        quad[3].texCoords = sf::Vector2f(0, levelHeight);

        for (int i = 0; i < 4; i++)
            backgroundVertices.append(quad[i]);
    }
}

//======================+Tuyau================================
void Level::handleTuyauInteraction(Player &mario, Player *luigi, float deltaTime)
{
    if (!enTrainDeDescendre && !enTrainDeMonter)
    {
        for (auto &bloc : blocs)
        {
            Tuyau *tuyau = dynamic_cast<Tuyau *>(bloc.get());
            if (tuyau && tuyau->getType() == Tuyau::Type::ENTREE)
            {

                bool marioSurTuyau = tuyau->isPlayerOnTop(mario) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
                bool luigiSurTuyau = luigi && tuyau->isPlayerOnTop(*luigi) && sf::Keyboard::isKeyPressed(sf::Keyboard::S);

                if (marioSurTuyau || luigiSurTuyau)
                {
                    enTrainDeDescendre = true;
                    tuyauTimer = 0.0f;

                    Tuyau *sortie = tuyau->getSortieAssociee(blocs);
                    if (sortie)
                    {
                        sortiePosition = sortie->getPosition();
                    }

                    sf::Vector2f tuyauPos = tuyau->getPosition();
                    sf::FloatRect tuyauBounds = tuyau->getGlobalBounds();
                    sf::FloatRect marioBounds = mario.getHitbox();

                    float marioNewX = tuyauPos.x + (tuyauBounds.width - marioBounds.width) / 2.0f;
                    mario.setPosition(marioNewX, mario.getPosition().y);

                    // Centrer Luigi seulement en mode multijoueur
                    if (luigi)
                    {
                        sf::FloatRect luigiBounds = luigi->getHitbox();
                        float luigiNewX = tuyauPos.x + (tuyauBounds.width - luigiBounds.width) / 2.0f;
                        luigi->setPosition(luigiNewX, luigi->getPosition().y);
                    }
                    return;
                }
            }
        }
    }

    // Animation de descente
    if (enTrainDeDescendre)
    {
        tuyauTimer += deltaTime;
        float descenteSpeed = 100.0f;

        mario.move(0, descenteSpeed * deltaTime);
        if (luigi)
            luigi->move(0, descenteSpeed * deltaTime);

        float alpha = 255 * (1.0f - (tuyauTimer / 0.5f));
        mario.setOpacity(static_cast<sf::Uint8>(std::max(0.0f, alpha)));
        if (luigi)
            luigi->setOpacity(static_cast<sf::Uint8>(std::max(0.0f, alpha)));

        if (tuyauTimer >= 0.5f)
        {
            mario.setPosition(sortiePosition.x, sortiePosition.y + 64.0f);
            if (luigi)
                luigi->setPosition(sortiePosition.x, sortiePosition.y + 74.0f);

            mario.setOpacity(0);
            if (luigi)
                luigi->setOpacity(0);

            enTrainDeDescendre = false;
            enTrainDeMonter = true;
            tuyauTimer = 0.0f;
        }
    }

    // Animation de montée
    if (enTrainDeMonter)
    {
        tuyauTimer += deltaTime;
        float monteeSpeed = 100.0f;

        mario.move(0, -monteeSpeed * deltaTime);
        if (luigi)
            luigi->move(0, -monteeSpeed * deltaTime);

        float alpha = 255 * (tuyauTimer / 0.5f);
        mario.setOpacity(static_cast<sf::Uint8>(std::min(255.0f, alpha)));
        if (luigi)
            luigi->setOpacity(static_cast<sf::Uint8>(std::min(255.0f, alpha)));

        if (tuyauTimer >= 0.5f)
        {
            enTrainDeMonter = false;
            tuyauTimer = 0.0f;

            mario.setOpacity(255);
            if (luigi)
            {
                luigi->setOpacity(255);
            }
        }
    }
}
