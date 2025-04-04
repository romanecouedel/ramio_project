#include "Player.h"
#include "Level.h"
#include "Animation.h"
#include "Audio.h"
#include <iostream>

extern AudioManager audioManager;

//=========================================Classe Player=====================================//
/**
 * @brief Constructeur par défaut du joueur.
 * 
 * Initialise l'animation par défaut du joueur à l'état d'attente vers la droite.
 */
Player::Player()
{
    currentAnimation = &animationIdleRight; // animation par défaut
}

/**
 * @brief Réinitialise la position et l'état du joueur après une mort.
 */
void Player::respawn()
{
    sprite.setPosition(100, 100); // position de base
    velocity = {0, 0};            // Réinitialiser la vitesse
    isDead = false;
}


/**
 * @brief Dessine le joueur sur la fenêtre SFML.
 * 
 * @param window Fenêtre SFML où dessiner le joueur.
 */
void Player::draw(sf::RenderWindow &window) const
{
    window.draw(sprite);
}

/**
 * @brief Met à jour l'état du joueur en fonction du temps écoulé et du niveau.
 * 
 * Cette fonction gère :
 * - La gravité et le mouvement du joueur.
 * - La détection des collisions avec le niveau.
 * - Le changement d'animation en fonction de l'état du joueur.
 * - La gestion des ennemis.
 * 
 * @param deltaTime Temps écoulé depuis la dernière frame.
 * @param level Référence vers le niveau pour la gestion des collisions.
 */
void Player::update(float deltaTime, const Level &level) {
    // Vérifie si le joueur est tombé hors du niveau (mort)
    if (sprite.getPosition().y > level.getHeight() * 64 + 100) { 
        isDead = true;
        return;
    }
    // Vérifie si le joueur est dans l'eau
    bool isInWater = false;
    for (const auto& bloc : level.getBlocs()) {
        if (auto* eau = dynamic_cast<Eau*>(bloc.get())) {
            if (eau->isPlayerInWater(getGlobalBounds())) {
                isInWater = true;
                break;  // Le joueur est dans l'eau, on peut arrêter la recherche
            }
        }
    }
    if (isInWater) {
        gravity = 50.f;  
        speed = 100.f;   
        jumpForce = -100.f; 

        // On incrémente le timer de cooldown uniquement si le joueur est dans l'eau
        jumpCooldown += deltaTime;

        // Si 2 secondes sont passées, on permet de sauter
        if (jumpCooldown >= 0.2f) {
            canJump = true;  // Permet au joueur de sauter
            jumpCooldown=0.0f;
        }
    } else {// si le joueur n'est pas dans l'eau 
        gravity = 980.f;  
        speed = 350.f;    
        jumpForce = -650.f; 
        
        jumpCooldown = 0.0f;
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
    checkCollisionWithEnnemis(level.getEnnemis());
    
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


/**
 * @brief Modifie l'opacité du sprite du joueur.
 * 
 * Cette fonction est utilisée notamment lors du passage dans un tuyau.
 * 
 * @param alpha Valeur de l'opacité (0 = transparent, 255 = opaque).
 */
void Player::setOpacity(sf::Uint8 alpha)
{
    sf::Color color = sprite.getColor();
    color.a = alpha;
    sprite.setColor(color);
}


/**
 * @brief Fait sauter le joueur.
 * 
 * Applique une force vers le haut et change l'animation pour celle du saut.
 */

void Player::jump() {
    
        // Si on n'est pas dans l'eau, on applique le saut normalement
        velocity.y = jumpForce;
        canJump = false;
        currentAnimation = faceRight ? &animationJumpRight : &animationJumpLeft;
        audioManager.playYahooSound();
    
}

/**
 * @brief Initialise le joueur avec une texture et une position de départ.
 * 
 * Charge la texture, définit la position et initialise les animations.
 * 
 * @param texturePath Chemin vers le fichier de texture.
 * @param position Position initiale du joueur.
 */
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

/**
 * @brief Vérifie les collisions entre le joueur et les ennemis.
 * 
 * Cette fonction détecte si le joueur entre en contact avec un ennemi et gère les conséquences :
 * - Si le joueur atterrit sur l'ennemi par le haut, il rebondit et l'ennemi est éliminé.
 * - Si la collision est latérale, le joueur meurt.
 * 
 * @param ennemis Vecteur contenant les ennemis présents dans le niveau.
 */
void Player::checkCollisionWithEnnemis(const std::vector<std::unique_ptr<Ennemi>>& ennemis)
{
    for (const auto& ennemi : ennemis)
    {
        sf::FloatRect ennemiBounds = ennemi->getBounds();
        sf::FloatRect playerBounds = getGlobalBounds();

        if (playerBounds.intersects(ennemiBounds))
        {
            float playerFeet = playerBounds.top + playerBounds.height; // Position des pieds du joueur
            float ennemiTop = ennemiBounds.top + (ennemiBounds.height * 0.2f); // Marge pour éviter les faux positifs

            if (playerFeet < ennemiTop) //  Ramio saute bien sur l'ennemi
            {
                velocity.y = -300.f; // Fait rebondir Ramio après un saut sur l'ennemi
                ennemi->onPlayerCollision(true);
            }
            else //  Collision latérale = mort
            {
                std::cout << "Ramio est mort en touchant l'ennemi !" << std::endl;
                isDead = true;
            }
        }
    }
}


//==========================Classes Filles==========================//
//===============================================Ramio=============================================/

/**
 * @brief Constructeur de la classe Ramio.
 * 
 * Initialise Ramio avec sa texture et sa position de départ.
 */
Ramio::Ramio()
{
    initializePlayer("../img/sprite_ramio.png", {100, 100});
}

/**
 * @brief Gère les entrées clavier pour déplacer Ramio.
 * 
 * - Gauche : Flèche gauche.
 * - Droite : Flèche droite.
 * - Saut : Flèche haut (uniquement si Ramio peut sauter).
 */
void Ramio::handleInput()
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
/**
 * @brief Constructeur de la classe Guili.
 * 
 * Initialise Guili avec sa texture et sa position de départ.
 */
Guili::Guili()
{
    initializePlayer("../img/sprite_guili.png", {150, 100});
}


/**
 * @brief Gère les entrées clavier pour déplacer Guili.
 * 
 * - Gauche : Touche 'Q'.
 * - Droite : Touche 'D'.
 * - Saut : Touche 'Z' (uniquement si Guili peut sauter).
 */
void Guili::handleInput()
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


//=========================================IA de Guili=========================================//
/**
 * @brief Fait marcher Guili de manière normale en fonction de la position de Ramio et des obstacles.
 * 
 * - Guili suit Ramio sans le dépasser.
 * - Il saute s'il rencontre un obstacle ou un ennemi.
 * - Il adapte son mouvement en fonction de la direction de Ramio.
 */
void Guili::marcher_normal()
{
    sf::FloatRect hitbox = getGlobalBounds();

    // Vérifie si Guili est sur le point de dépasser Ramio
    if (ramio->faceRight)
    {
        hitbox.left += 0.25 * speed; // Simulation de la position future à droite
        if (sprite.getPosition().x > ramio->getPosition().x - 100)
        {
            velocity.x = 0; // Arrête Guili pour ne pas dépasser Ramio
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

        for (const auto& ennemi : level->getEnnemis())
        {
            sf::FloatRect ennemiBounds = ennemi->getBounds();
            if (hitbox.intersects(ennemiBounds) && canJump)
            {
                std::cout << "Guili détecte un ennemi et saute" << std::endl;
                jump();
            }
        }
    }
    else
    {
        hitbox.left -= 0.25 * speed; // Simulation de la position future à droite
        if (sprite.getPosition().x < ramio->getPosition().x + 100)
        {
            velocity.x = 0; // Arrête Guili pour ne pas dépasser Ramio
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
        for (const auto& ennemi : level->getEnnemis())
        {
            sf::FloatRect ennemiBounds = ennemi->getBounds();
            if (hitbox.intersects(ennemiBounds) && canJump)
            {
                std::cout << "Guili détecte un ennemi et saute" << std::endl;
                jump();
            }
        }
    }
}

/**
 * @brief Gère l'IA de Guili en fonction des objets du niveau.
 * 
 * Guili prend des décisions en fonction de :
 * - La position de Ramio.
 * - La présence de blocs mystères.
 * - Les obstacles ou ennemis sur son chemin.
 * 
 * @param lvl Pointeur vers le niveau actuel.
 * @param ramio Pointeur vers l'objet Ramio.
 */
void Guili::handleInputAI(Level *lvl, const Ramio *ramio)
{
    this->level = lvl;
    this->ramio = ramio;

    BlocMystere *blocProche = level->getBlocMystereProche(sprite.getPosition());
    if (blocProche != nullptr)
    {
        sf::Vector2f position = blocProche->getPosition();
        std::cout << "Bloc mystère détecté à (" << position.x << ", " << position.y << ")" << std::endl;
        std::cout << "touché :" << blocProche->estTouche << std::endl;
        if (position.x < sprite.getPosition().x && position.y > sprite.getPosition().y - 300.0f && !blocProche->estTouche)
        {
            // Chemin pour aller jusqu'à la boîte
            velocity.x = -0.5 * speed;
            faceRight = false;
            currentAnimation = &animationWalkLeft;
            std::cout << "Guili se déplace vers la gauche" << std::endl;
            // Vérifie si Guili a atteint le bloc mystère
            if (std::abs(sprite.getPosition().x - position.x) < 32.0f && sprite.getPosition().y > position.y)
            {
                // Je suis en dessous de la boîte
                std::cout << "Guili saute pour atteindre le bloc mystère" << std::endl;
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
