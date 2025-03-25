#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Ennemi.h"
#include "Level.h"
#include <cstdlib>
#include <ctime>

class Level;
class Ennemi;

// ============================================= Classe Player ===========================================$
// classe fille de Entity
// classe mère de ramio et guili

class Player : public Entity {
protected:
    // variables d'animation uniquement accessibles par les classes filles
    Animation animationWalkLeft;
    Animation animationWalkRight;
    Animation animationJumpRight;
    Animation animationJumpLeft;
    Animation animationIdleRight;
    Animation animationIdleLeft;
    Animation* currentAnimation;
    sf::Vector2f velocity;
    float jumpForce = -650.f;
    float speed = 350.f;
    float gravity = 980.f;

    // variables de saut, nottament utilisée pour l'ia de guili
    bool isJumping = false;
    bool canJump = true;
    bool onGround = false;


public:
    // constructeur
    Player();
    virtual ~Player() {
        std::cout << "Destruction de Player" << std::endl;
        
    }
    bool faceRight = true; // Direction du personnage, sert pour l'animation
    bool isDead = false;// sert à compter les morts
    bool collisionsActive = true; // Désactive la collision temporairement
    
    void draw(sf::RenderWindow& window) const override;// surchargée car méthode héritée de Entity
    void jump();
    void move(float dx, float dy) {sprite.move(dx, dy);}
    void initializePlayer(const std::string& texturePath, sf::Vector2f position);
    void checkCollisionWithEnnemis(const std::vector<std::unique_ptr<Ennemi>>& ennemis);    
    void respawn();

    void setCollisionsActive(bool active) { collisionsActive = active; }
    bool areCollisionsActive() const { return collisionsActive; }
    void setOpacity(sf::Uint8 alpha); // sert pour l'animation tuyau
    void update(float deltaTime, const Level& level); // mise à jour du joueur en fonction des collisions du level

    // Méthodes virtuelles pures, implémentées par les classes filles
    virtual void handleInput() = 0; 

    
};

// ============================================= Classe Ramio ===========================================
// classe fille de Player
// représente le personnage Ramio jouable uniquement par l'utilisateur
class Ramio : public Player {
public:
    // constructeur
    Ramio();
    ~Ramio() override {
        std::cout << "Destruction de Ramio" << std::endl;
    }
    // entrées clavier pour déplacer Ramio
    void handleInput() override; //polymorphisme car peut etre utilisé par guili ou ramio et faire des actions différentes
};

// ============================================= Classe Guili ===========================================
// classe fille de Player
// représente le personnage Guili pour un jeu 2 joueurs en local ou avec l'ordinateur
class Guili : public Player {
public:
    Level* level;
    const Ramio* ramio; // Référence à Ramio pour l'IA

    // constructeur
    Guili();
    ~Guili() override {
        std::cout << "Destruction de Guili" << std::endl;
        level = nullptr; 
        ramio = nullptr;
    } 

    // entrées clavier pour déplacer Guili en local
    void handleInput() override;//polymorphisme car peut etre utilisé par guili ou ramio et faire des actions différentes

    // déplacement de Guili en fonction de Ramio pour l'IA
    void marcher_normal();
    void handleInputAI(Level* lvl, const Ramio* ramio);

};

