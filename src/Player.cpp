// Player.cpp
#include "Player.h"
#include "Level.h"
#include "Animation.h"
#include "Audio.h"
#include <iostream>

extern AudioManager audioManager;

//=========================================Classe Player=====================================//
// constructeur
Player::Player()
{
    currentAnimation = &animationIdleRight; // animation par défaut
}

// faire revenir le sprite au point de départ après une mort
void Player::respawn()
{
    sprite.setPosition(100, 100); // position de base
    velocity = {0, 0};            // Réinitialiser la vitesse
    isDead = false;
}

// dessine le joueur sur la fenêtre
void Player::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

// mise à jour du joueur en fonction des collisions du level
// param : le temps écoulé depuis la dernière frame, le niveau
// Met à jour l'état du joueur en fonction du temps écoulé et du niveau
void Player::update(float deltaTime, const Level &level) {
    // Vérifie si le joueur est tombé hors du niveau (mort)
    if (sprite.getPosition().y > level.getHeight() * 64 + 100) { 
        isDead = true;
        return;
    }

    // Applique la gravité
    velocity.y += gravity * deltaTime;

    // Détecte si le joueur a atterri après un saut
    if (velocity.y > 0 && onGround) { 
        isJumping = false;
    }

    // Gère l'animation d'attente si le joueur est immobile au sol
    if (velocity.x == 0 && onGround) { 
        currentAnimation = faceRight ? &animationIdleRight : &animationIdleLeft;
        isJumping = false;
    }

    else if (velocity.y < 0)
    {
        isJumping = true;
    }

    // Met à jour l'animation du joueur
    currentAnimation->update(deltaTime, faceRight, isJumping);
    sprite.setTextureRect(currentAnimation->getCurrentFrame());

    // Gestion des collisions horizontales
    sf::FloatRect hitbox = getGlobalBounds();
    hitbox.left += velocity.x * deltaTime;
    if (!level.isColliding(hitbox)) {
        sprite.move(velocity.x * deltaTime, 0);
    } else {
        velocity.x = 0; // Stoppe le mouvement horizontal en cas de collision
    }

    // Gestion des collisions verticales
    hitbox = getGlobalBounds();
    hitbox.top += velocity.y * deltaTime;

    if (!level.isColliding(hitbox)) {
        sprite.move(0, velocity.y * deltaTime);
        onGround = false;
    } else {
        // Collision avec le plafond
        if (velocity.y < 0) {
            while (level.isColliding(getGlobalBounds())) {
                sprite.move(0, 0.5f); // Décale légèrement vers le bas pour éviter le blocage
            }
            velocity.y = 0;
        }
        // Collision avec le sol
        else if (velocity.y > 0) {
            while (level.isColliding(getGlobalBounds())) {
                sprite.move(0, -0.5f); // Décale légèrement vers le haut
            }
            velocity.y = 0;
            onGround = true;
            canJump = true;
            isJumping = false; 
        }
    }
}
// change l'opacité du sprite, utile pour l'animation du tuyau
// param : la valeur de l'opacité
void Player::setOpacity(sf::Uint8 alpha)
{
    sf::Color color = sprite.getColor();
    color.a = alpha;
    sprite.setColor(color);
}

// fait sauter le joueur
void Player::jump()
{
    velocity.y = jumpForce;
    canJump = false;
    currentAnimation = faceRight ? &animationJumpRight : &animationJumpLeft;
    audioManager.playYahooSound();
}

// initialise le joueur avec une texture et une position
// param : le chemin de la texture, la position
void Player::initializePlayer(const std::string &texturePath, sf::Vector2f position)
{
    if (!texture.loadFromFile(texturePath))
    {
        std::cerr << "Erreur chargement texture: " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.40f, 0.40f);
    sprite.setPosition(position);
    animationWalkRight = Animation(&texture, {1, 1}, 0.1f);
    animationWalkLeft = Animation(&texture, {1, 2}, 0.1f);
    animationJumpLeft = Animation(&texture, {4, 1}, 0.1f);
    animationJumpRight = Animation(&texture, {2, 1}, 0.1f);
    animationIdleRight = Animation(&texture, {1, 1}, 0.f);
    animationIdleLeft = Animation(&texture, {1, 2}, 0.f);
    currentAnimation = &animationIdleRight;
}

//==========================Classes Filles==========================//
//===============================================Mario=============================================/

Mario::Mario()
{
    initializePlayer("../img/sprite_mario.png", {100, 100});
}

void Mario::handleInput()
{
    velocity.x = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        velocity.x = -speed;
        faceRight = false;
        currentAnimation = &animationWalkLeft;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        velocity.x = speed;
        faceRight = true;
        currentAnimation = &animationWalkRight;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canJump)
        jump();
}

//==================================================LUIGI==========================================//

Luigi::Luigi()
{
    initializePlayer("../img/sprite_luigi.png", {150, 100});
}

void Luigi::handleInput()
{
    velocity.x = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        velocity.x = -speed;
        faceRight = false;
        currentAnimation = &animationWalkLeft;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        velocity.x = speed;
        faceRight = true;
        currentAnimation = &animationWalkRight;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && canJump)
    {
        jump();
    }
}


//=========================================IA de Luigi=========================================//
void Luigi::marcher_normal()
{
    sf::FloatRect hitbox = getGlobalBounds();

    // Vérifie si Luigi est sur le point de dépasser Mario
    if (mario->faceRight)
    {
        hitbox.left += 0.25 * speed; // Simulation de la position future à droite
        if (sprite.getPosition().x > mario->getPosition().x - 100)
        {
            velocity.x = 0; // Arrête Luigi pour ne pas dépasser Mario
        }
        else
        {
            velocity.x = speed;
            faceRight = true;
            currentAnimation = &animationWalkRight;
        }
        if (level->isColliding(hitbox) && canJump)
        {
            jump();
            velocity.x = speed; // Continue à avancer vers la droite après le saut
            faceRight = true;
            currentAnimation = &animationJumpRight;
        }
        if (level->isTuyauColliding(hitbox))
        {
            velocity.x = speed; // Continue à avancer vers la droite après le saut
            faceRight = true;
            currentAnimation = &animationJumpRight;
        }
    }
    else
    {
        hitbox.left -= 0.25 * speed; // Simulation de la position future à droite
        if (sprite.getPosition().x < mario->getPosition().x + 100)
        {
            velocity.x = 0; // Arrête Luigi pour ne pas dépasser Mario
        }
        else
        {
            velocity.x = -speed;
            faceRight = false;
            currentAnimation = &animationWalkLeft;
        }
        if (level->isColliding(hitbox) && canJump)
        {
            jump();
            velocity.x = -speed; // Continue à avancer vers la gauche après le saut
            faceRight = false;
            currentAnimation = &animationJumpLeft;
        }
        if (level->isTuyauColliding(hitbox) && canJump)
        {
            velocity.x = -speed; // Continue à avancer vers la droite après le saut
            faceRight = false;
            currentAnimation = &animationJumpRight;
        }
    }
}

// IA de Luigi
void Luigi::handleInputAI(Level *lvl, const Mario *mario)
{
    this->level = lvl;
    this->mario = mario;
    BlocMystere *blocProche = level->getBlocMystereProche(sprite.getPosition());
    if (blocProche != nullptr)
    {
        sf::Vector2f position = blocProche->getPosition();
        std::cout << "Bloc mystère détecté à (" << position.x << ", " << position.y << ")" << std::endl;
        std::cout << "touché :" << blocProche->estTouche << std::endl;
        if (position.x < sprite.getPosition().x && position.y > sprite.getPosition().y - 300.0f && !blocProche->estTouche)
        {
            // Chemin pour aller jusqu'à la boîte
            velocity.x = -0.25 * speed;
            faceRight = false;
            currentAnimation = &animationWalkLeft;
            std::cout << "Luigi se déplace vers la gauche" << std::endl;
            // Vérifie si Luigi a atteint le bloc mystère
            if (std::abs(sprite.getPosition().x - position.x) < 32.0f && sprite.getPosition().y > position.y)
            {
                // Je suis en dessous de la boîte
                std::cout << "Luigi saute pour atteindre le bloc mystère" << std::endl;
                jump();
            }
        }
        else
        {
            marcher_normal();
        }
    }
    else
    {
        marcher_normal();
    }
}
