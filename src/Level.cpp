#include "Level.h"
#include "Confetti.h"
#include "Bloc.h"
#include "Player.h"
#include "Audio.h"
#include <fstream>
#include <iostream>
#include <cmath>

extern AudioManager audioManager;

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
            else if (c == 'X' || c == 'K')
            {
                Ennemi ennemi;
                ennemi.setPosition(position.x, position.y);
                ennemis.push_back(ennemi);
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
    return true;
}

// ======================== Affichage du Niveau ========================
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

    generateBackground(grid[0].size() * 64, grid.size() * 64);

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

    drawEnnemis(window);
    drapeau.draw(window);
}

// ======================== Mise à Jour ========================
void Level::update(float deltaTime, sf::RenderWindow &window, const sf::FloatRect &hitboxMario, const sf::FloatRect &hitboxLuigi)
{
    updateEnnemis(deltaTime);

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
            if (hitboxMario.intersects(hitboxAvecTolerance) || hitboxLuigi.intersects(hitboxAvecTolerance))
            {
                float milieuBloc = blocMystere->getGlobalBounds().top + blocMystere->getGlobalBounds().height * 0.4f;
                if (!blocMystere->isAnimating())
                {
                    if (hitboxMario.top > milieuBloc || hitboxLuigi.top > milieuBloc)
                    {
                        blocMystere->onHit();
                        
                        blocMystere->estTouche=true; //provoque segmentation fault
                    }
                }
            }
        }
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

//==========================detection bloc mystere proche==========================

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
BlocMystere* Level::getBlocMystereProche(const sf::Vector2f& position) {
    float blockSize = 64.0f; // Taille d'un bloc dans ton niveau
    float tolerance = 500.0f; // Tolérance pour la vérification des positions

    // Définition des directions autour du joueur (haut, bas, gauche, droite)
    std::vector<sf::Vector2f> directions = {
        {0, -blockSize},  // Au-dessus
        {0, blockSize},   // En dessous
        {-blockSize, 0},  // À gauche
        {blockSize, 0}    // À droite
    };

    for (const auto& dir : directions) {
        sf::Vector2f checkPosition = position + dir;

        for (const auto& bloc : blocs) {
            if (auto* blocMystere = dynamic_cast<BlocMystere*>(bloc.get())) {
                sf::Vector2f blocPosition = blocMystere->getPosition();
                if (std::abs(blocPosition.x - checkPosition.x) < tolerance &&
                    std::abs(blocPosition.y - checkPosition.y) < tolerance) {
                    // Vérifie si le bloc mystère contient encore un objet (non vide)
                    if (!blocMystere->estTouche) {
                        return blocMystere;
                    }
                }
            }
        }
    }

    return nullptr; // Aucun bloc mystère valide trouvé
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

//===========================ENNEMI===================================

void Level::updateEnnemis(float deltaTime)
{
    for (auto &ennemi : ennemis)
    {
        ennemi.update(deltaTime); // Met à jour leur mouvement
    }
}

void Level::drawEnnemis(sf::RenderWindow &window)
{
    for (auto &ennemi : ennemis)
    {
        ennemi.draw(window);
    }
}

//======================+Tuyau================================
void Level::handleTuyauInteraction(Player &player, float deltaTime)
{
    // Si le joueur n'est pas en train d'entrer ou de sortir d'un tuyau
    if (!enTrainDeDescendre && !enTrainDeMonter)
    {
        for (auto &bloc : blocs)
        {
            Tuyau *tuyau = dynamic_cast<Tuyau *>(bloc.get());
            if (tuyau && tuyau->getType() == Tuyau::Type::ENTREE)
            {
                if (tuyau->isPlayerOnTop(player) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {

                    // Initialisation de l'animation de descente
                    enTrainDeDescendre = true;
                    tuyauTimer = 0.0f;

                    // Centrer le joueur horizontalement sur le tuyau
                    sf::Vector2f tuyauPos = tuyau->getPosition();
                    sf::FloatRect tuyauBounds = tuyau->getGlobalBounds();
                    sf::FloatRect playerBounds = player.getHitbox();
                    float newX = tuyauPos.x + (tuyauBounds.width / 2.0f) - (playerBounds.width / 2.0f);
                    player.setPosition(newX, player.getPosition().y);
                    player.setCollisionsActive(false);
                    // Chercher la sortie
                    for (auto &bloc2 : blocs)
                    {
                        Tuyau *sortie = dynamic_cast<Tuyau *>(bloc2.get());
                        if (sortie && sortie->getType() == Tuyau::Type::SORTIE)
                        {
                            sortiePosition = sortie->getPosition();
                            return;
                        }
                    }
                }
            }
        }
    }

    /// Animation de descente
    if (enTrainDeDescendre)
    {
        tuyauTimer += deltaTime;
        float descenteSpeed = 80.0f; // 🔽 Plus lent pour mieux voir
        player.move(0, descenteSpeed * deltaTime);

        // Réduire progressivement l'opacité (disparition plus naturelle)
        float fadeStart = 0.2f; // Temps avant que l'opacité commence à diminuer
        if (tuyauTimer >= fadeStart)
        {
            float alpha = 255 * (1.0f - (tuyauTimer - fadeStart) / 0.5f); // Transition plus douce
            player.setOpacity(static_cast<sf::Uint8>(std::max(0.0f, alpha)));
        }

        if (tuyauTimer >= 0.6f)
        { // ⏳ Temps total augmenté un peu
            sf::Vector2f sortiePos = sortiePosition;
            sf::FloatRect sortieBounds = player.getHitbox();
            float newXSortie = sortiePos.x + (64.0f / 2.0f) - (sortieBounds.width / 2.0f);

            player.setPosition(newXSortie, sortiePos.y + 64.0f);
            player.setOpacity(0);

            enTrainDeDescendre = false;
            enTrainDeMonter = true;
            tuyauTimer = 0.0f;
        }
    }

    // Animation de montée
    if (enTrainDeMonter)
    {
        tuyauTimer += deltaTime;
        float monteeSpeed = 80.0f; // 🔼 Plus lent aussi
        player.move(0, -monteeSpeed * deltaTime);

        // Opacité augmente progressivement mais commence après 0.2s
        float fadeStart = 0.2f;
        if (tuyauTimer >= fadeStart)
        {
            float alpha = 255 * ((tuyauTimer - fadeStart) / 0.5f);
            player.setOpacity(static_cast<sf::Uint8>(std::min(255.0f, alpha)));
        }

        if (tuyauTimer >= 0.6f)
        {
            enTrainDeMonter = false;
            tuyauTimer = 0.0f;
            player.setOpacity(255);
            player.setCollisionsActive(true);
        }
    }
}
bool Level::isTuyauColliding(const sf::FloatRect& hitbox) const {
    for (const auto& bloc : blocs) {
        if (auto* tuyau = dynamic_cast<Tuyau*>(bloc.get())) {
            if (hitbox.intersects(tuyau->getGlobalBounds())) {
                return true;
            }
        }
    }
    return false;
}