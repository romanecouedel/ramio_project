#include "Ennemi.h"
#include <iostream>

/**
 * @brief Texture statique partagée entre tous les ennemis.
 */
sf::Texture Ennemi::textureEnnemi;

/**
 * @brief Constructeur de l'ennemi Goomba.
 * Initialise la texture, la taille de l'animation et la vitesse de déplacement.
 */
Ennemi::Ennemi()
{
    if (textureEnnemi.getSize().x == 0) // Charge la texture une seule fois
    {
        if (!textureEnnemi.loadFromFile("../img/goomba.png"))
        {
            std::cerr << "Erreur chargement texture Ennemi" << std::endl;
        }
    }
    sprite.setTexture(textureEnnemi);

    // Calcul automatique de la taille d'une frame
    frameWidth = textureEnnemi.getSize().x / 3; // 3 images sur la ligne
    frameHeight = textureEnnemi.getSize().y;

    // Appliquer la première image
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

    sprite.setScale(64.0f / frameWidth, 64.0f / frameHeight);

    velocity.x = -50.f;
    velocity.y = 0.f;
    isSquashed = false;
    isAlive = true;
}

/**
 * @brief Définit la position de l'ennemi.
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 */
void Ennemi::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

/**
 * @brief Met à jour l'état et le mouvement de l'ennemi.
 * @param deltaTime Temps écoulé depuis la dernière mise à jour.
 * @param level Référence au niveau pour gérer les collisions.
 */
void Ennemi::update(float deltaTime, Level &level)
{
    if (isSquashed)
    {
        squashTimer += deltaTime;
        if (squashTimer >= 0.05f) // Après 1 seconde, l'ennemi disparaît
        {
            isAlive = false;
        }
        return;
    }

    // Animation de marche (2 premières images)
    animationTimer += deltaTime;
    if (animationTimer > 0.2f)
    {
        animationTimer = 0.f;
        currentFrame = (currentFrame + 1) % 2;
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
    }

    // Gestion des collisions latérales
    sf::FloatRect hitbox = sprite.getGlobalBounds();
    sf::Vector2f leftPoint(hitbox.left - 2.0f, hitbox.top + hitbox.height / 2);
    sf::Vector2f rightPoint(hitbox.left + hitbox.width + 2.0f, hitbox.top + hitbox.height / 2);

    if (velocity.x < 0 && level.isColliding(sf::FloatRect(leftPoint, {1.0f, 1.0f})))
    {
        velocity.x = 50.f;
    }
    else if (velocity.x > 0 && level.isColliding(sf::FloatRect(rightPoint, {1.0f, 1.0f})))
    {
        velocity.x = -50.f;
    }

    // Gestion de la gravité
    velocity.y += gravity * deltaTime;
    sf::FloatRect hitboxBelow(hitbox.left, hitbox.top + hitbox.height, hitbox.width, 5.0f);

    if (level.isColliding(hitboxBelow))
    {
        velocity.y = 0;
        onGround = true;
    }
    else
    {
        onGround = false;
    }

    //Supprimer le Goomba s'il tombe hors de l'écran
    if (sprite.getPosition().y > 2000) // Ajuste cette valeur selon la taille de ton niveau
    {
        isAlive = false;
    }

    // appliquer le mouvement (évite le double `sprite.move()`)
    sprite.move(velocity * deltaTime);
}

/**
 * @brief Gère la collision avec le joueur.
 * @param fromAbove Indique si la collision vient du dessus (pour écraser l'ennemi).
 */
void Ennemi::onPlayerCollision(bool fromAbove)
{
    if (fromAbove)
    {
        isSquashed = true;
        sprite.setTextureRect(sf::IntRect(2 * frameWidth, 0, frameWidth, frameHeight)); // Image écrasée
        velocity.x = 0;
    }
}

/**
 * @brief Dessine l'ennemi à l'écran.
 * @param window Fenêtre d'affichage.
 */
void Ennemi::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

/**
 * @brief Obtient la boîte englobante de l'ennemi.
 * @return Rectangle de collision de l'ennemi.
 */
sf::FloatRect Ennemi::getBounds() const
{
    return sprite.getGlobalBounds();
}
