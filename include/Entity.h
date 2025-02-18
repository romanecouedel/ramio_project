// Entity.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Entity {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    float speed = 200.f;
    float gravity = 980.f;
    bool canJump = false;
    bool onGround = false;

public:
    Entity() = default;  
    explicit Entity(const std::string& texturePath); // Ajout du constructeur
    
    virtual ~Entity() = default;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    void move(float dx, float dy); // Ajout de move()
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    
    sf::FloatRect getGlobalBounds() const; // Déclaration ici, suppression du inline
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    
};

// ===================== Classe ObjetInteractif =====================
class ObjetInteractif : public Entity {
public:
    ObjetInteractif(const std::string& texturePath);
    void update(float deltaTime) override {}
    void draw(sf::RenderWindow& window) const override {
        window.draw(sprite);
    }
};

// ===================== Classe Drapeau =====================
class Drapeau : public ObjetInteractif {
public:
    Drapeau(); // Déclaration seulement
};