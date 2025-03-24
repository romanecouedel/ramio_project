#include "Level.h"
#include "Bloc.h"
#include "Player.h"
#include "Audio.h"
#include <fstream>
#include <iostream>
#include <cmath>

extern AudioManager audioManager;


/**
 * @brief Constructeur de la classe Level
 */
Level::Level()
{

    if (!bgTextureLeft.loadFromFile("../img/background1.png") ||
        !bgTextureRight.loadFromFile("../img/background2.png"))
    {
        std::cerr << "Erreur chargement textures de fond" << std::endl;
    }
    bgWidth = bgTextureLeft.getSize().x; // Largeur d'une image

    // Créer un vertex array pour dessiner le fond
    backgroundVertices.setPrimitiveType(sf::Quads);
}

// ======================== Chargement du Niveau ========================
/**
 * @brief Charge un niveau à partir d'un fichier
 * @param filename Nom du fichier de niveau
 * @return true si le chargement a réussi, false sinon
 */
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
    ennemis.clear();

    const float blockSize = 64.0f;
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

            std::unique_ptr<Bloc> bloc;
            auto ennemi = std::make_unique<Ennemi>();
            switch (c)
            {
            case '#':
                bloc = std::make_unique<BlocSol>();
                bloc->setPosition(position.x, position.y);
                blocs.push_back(std::move(bloc));
                break;
            case '?':
                bloc = std::make_unique<BlocMystere>();
                bloc->setPosition(position.x, position.y);
                blocs.push_back(std::move(bloc));
                break;
            case '!':
                drapeau.setPosition(position.x, position.y);
                break;
            case 'X': case 'K':
                ennemi->setPosition(position.x, position.y);
                ennemis.push_back(std::move(ennemi));
                break;
            case 'U': case 'V':
                Tuyau::Type type = (c == 'U') ? Tuyau::Type::ENTREE : Tuyau::Type::SORTIE;
                auto tuyau = std::make_unique<Tuyau>(type);
                tuyau->setPosition(position.x, position.y);
                blocs.push_back(std::move(tuyau));
                break;
            }
            }
        grid.push_back(row);
        ++y;
    }
    if (!grid.empty())
    {
        generateBackground(grid[0].size() * 64, grid.size() * 64);
    }
    std::cout << "Nombre d'ennemis chargés : " << ennemis.size() << std::endl;
    return true;
}


// ======================== Affichage du Niveau ========================
/**
 * @brief Affiche le niveau dans la fenêtre SFML
 * @param window Fenêtre de rendu
 */
void Level::draw(sf::RenderWindow &window)
{
    // Dessiner l'arrière-plan en utilisant un sf::RenderStates pour appliquer la texture
    sf::RenderStates statesLeft;
    statesLeft.texture = &bgTextureLeft;
    sf::RenderStates statesRight;
    statesRight.texture = &bgTextureRight;

    // Dessiner séparément chaque moitié
    for (size_t i = 0; i < backgroundVertices.getVertexCount(); i += 4)
    {
        if ((i / 4) % 2 == 0)
            window.draw(&backgroundVertices[i], 4, sf::Quads, statesLeft);
        else
            window.draw(&backgroundVertices[i], 4, sf::Quads, statesRight);
    }

    // generateBackground(grid[0].size() * 64, grid.size() * 64);

    // Dessiner les blocs
    for (const auto &bloc : blocs)
    {
        bloc->draw(window);

        // Vérifier si c'est un BlocMystere et dessiner la pièce s'il en a une
        if (auto *blocMystere = dynamic_cast<BlocMystere *>(bloc.get()))
        {
            if (blocMystere->isAnimating() && blocMystere->getPiece())
            {
                blocMystere->getPiece()->draw(window);
                audioManager.playCoinSound();
            }
        }
    }
    for (const auto &ennemi : ennemis)
    {
        ennemi->draw(window);
    }
    drapeau.draw(window);
}

// ======================== Mise à Jour ========================
/**
 * @brief Met à jour l'état du niveau
 * @param deltaTime Temps écoulé depuis la dernière mise à jour
 * @param window Fenêtre SFML
 * @param hitboxMario Hitbox de Mario
 * @param hitboxLuigi Hitbox de Luigi
 */
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

            // Vérifie si Mario ou Luigi frappe le bloc mystère
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
    for (auto &ennemi : ennemis)
    {
        ennemi->update(deltaTime, *this);

        sf::FloatRect ennemiBounds = ennemi->getBounds();

        for (const auto &hitboxJoueur : {hitboxMario, hitboxLuigi})
        {
            if (hitboxJoueur.intersects(ennemiBounds))
            {
                if (hitboxJoueur.top + hitboxJoueur.height - 5.0f < ennemiBounds.top)
                {
                    ennemi->onPlayerCollision(true);
                }
            }
        }
    }
    for (auto it = ennemis.begin(); it != ennemis.end();)
    {
        if (!it->get()->isAlive) // Accès explicite avec `get()`
            it = ennemis.erase(it); // Supprime et récupère le nouvel itérateur valide
        else
            ++it; // Passe à l'ennemi suivant
    }
}

// ======================== Détection de Collisions ========================
/**
 * @brief Vérifie si une hitbox est en collision avec un bloc solide du niveau.
 * 
 * @param hitbox La hitbox du joueur ou de l'objet à tester.
 * @return true si une collision est détectée, false sinon.
 */
bool Level::isColliding(const sf::FloatRect &hitbox) const
{
    // on regarde les collisions seulement autour du joueur
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
/**
 * @brief Affiche l'état des blocs mystères dans le niveau.
 * 
 * Affiche la position et l'état (touché ou non) de chaque bloc mystère.
 */
void Level::afficherEtatBlocsMysteres() const {
    for (const auto& bloc : blocs) {
        if (auto* blocMystere = dynamic_cast<BlocMystere*>(bloc.get())) {
            std::cout << "Bloc Mystere à (" << blocMystere->getPosition().x << ", " << blocMystere->getPosition().y << ") - ";
            if (blocMystere->estTouche) {
                std::cout << "Touché" << std::endl;
            } else {
                std::cout << "Non touché" << std::endl;
            }
        }
    }
}

//==========================detection bloc mystere proche==========================
/**
 * @brief Récupère un bloc mystère proche d'une position donnée.
 * 
 * @param position La position à partir de laquelle chercher.
 * @return Un pointeur vers le bloc mystère trouvé, ou nullptr si aucun n'est proche.
 */
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

//debug
void Level::afficherEtatBlocsMysteres() const {
    for (const auto& bloc : blocs) {
        if (auto* blocMystere = dynamic_cast<BlocMystere*>(bloc.get())) {
            std::cout << "Bloc Mystere à (" << blocMystere->getPosition().x << ", " << blocMystere->getPosition().y << ") - ";
            if (blocMystere->estTouche) {
                std::cout << "Touché" << std::endl;
            } else {
                std::cout << "Non touché" << std::endl;
            }
        }
    }
}


// ======================== Génération du Fond ========================
/**
 * @brief Génère le fond du niveau en alternant des textures.
 * @param levelWidth Largeur du niveau.
 * @param levelHeight Hauteur du niveau.
 */
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
/**
 * @brief Gère l'interaction des joueurs avec les tuyaux.
 * @param mario Le joueur principal.
 * @param luigi Le second joueur (optionnel).
 * @param deltaTime Temps écoulé depuis la dernière mise à jour.
 */
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
                    sf::FloatRect marioBounds = mario.getGlobalBounds();

                    float marioNewX = tuyauPos.x + (tuyauBounds.width - marioBounds.width) / 2.0f;
                    mario.setPosition(marioNewX, mario.getPosition().y);

                    // Centrer Luigi seulement en mode multijoueur
                    if (luigi)
                    {
                        sf::FloatRect luigiBounds = luigi->getGlobalBounds();
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

/**
 * @brief Vérifie si le joueur est en collision avec un tuyau.
 * @param playerBound Les limites du joueur.
 * @return true si une collision est détectée, sinon false.
 */
bool Level::isTuyauColliding(const sf::FloatRect &playerBound) const {
    sf::FloatRect tuyauBounds; // Initialize tuyauBounds
    for (const auto& bloc : blocs) {
        if (auto* tuyau = dynamic_cast<Tuyau*>(bloc.get())) {
            tuyauBounds = tuyau->getGlobalBounds(); // Use Tuyau's getGlobalBounds method
            break; // Assuming you only need the first Tuyau's bounds
        }
    }

    return playerBound.intersects(tuyauBounds);
}
