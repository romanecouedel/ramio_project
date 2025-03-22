#ifndef ENTITY_H
#define ENTITY_H


#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <math.h>


// ============================================= Classe Entity ===========================================
// est ce que la classe entity est une classe abstraite ?
// classe mère de toutes les entités du jeu, c'est à dire les objets interactifs, les personnages, etc.
class Entity {
protected://permet d'etre accessible depuis les classes filles
    sf::Sprite sprite;
    sf::Texture texture;

public:
    Entity() = default;  // nécessaire pour les constructeurs des classes filles
    explicit Entity(const std::string& texturePath); // permet à une classe fille de créer une entité avec une texture
    
    virtual ~Entity() = default; // destructeur virtuel garantit que lorsque l'objet est supprimé via un pointeur de la classe mere, le bon destructeur de la classe fille est appelé
    virtual void draw(sf::RenderWindow& window) const = 0; // méthode virtuelle pure, doit être implémentée par les classes filles
    
    // Méthodes communes à toutes les entités
    void setPosition(float x, float y) { sprite.setPosition(x, y); }
    
    sf::FloatRect getGlobalBounds() const; // retourne le rectangle englobant de l'entité
    sf::Vector2f getPosition() const { return sprite.getPosition(); } // retourne la position de l'entité
    
};

// ======================================== Classe ObjetInteractif =====================================
// classe fille de Entity, représente un objet interactif
// classe mère de pièces et drapeau

class ObjetInteractif : public Entity {
public:
    // constructeur 
    ObjetInteractif(const std::string& texturePath);
    // méthode draw qui dessine l'objet interactif
    void draw(sf::RenderWindow& window) const override {
        window.draw(sprite);
    }
};

// ========================================== Classe Drapeau ==========================================
// classe fille de ObjetInteractif, représente un drapeau de fin de niveau
class Drapeau : public ObjetInteractif {
public:
    Drapeau(); // constructeur
};

// ========================================== Classe Piece ===========================================
// classe fille de ObjetInteractif, représente une pièce à collecter
class Piece : public ObjetInteractif {
public:
    // constructeur
    Piece(float x, float y);
    static int getNbPiece(); // retourne le nombre de pièces collectées, appelé par le main pour l'affichage fin de jeu
    static void resetNbPiece(); // remet le nb de piece à zeros, appelé par le main
    void update(float deltaTime); // met à jour l'animation de la pièce
    void draw(sf::RenderWindow& window) const;

private:
    // variable d'animation
    sf::Vector2f startPosition;
    bool collected = 0;
    bool animating = false;
    float animationTime = 0.0f;
    const float animationDuration = 0.5f;  // Durée totale montée/descente
    const float animationHeight = 10.0f;   // Hauteur du déplacement

    // Variable statique pour compter le nombre de pièces collectées, récupéré par get et reset (méthodes publiques)
    static int nbPiece;

};

#endif // ENTITY_H