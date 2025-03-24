#include "Animation.h"

/**
 * @brief Constructeur de l'animation.
 * @param texture Pointeur vers la texture utilisée pour l'animation.
 * @param imageCount Nombre d'images par ligne et colonne dans la spritesheet.
 * @param switchTime Temps entre chaque changement de frame.
 */
Animation::Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime)
    : imageCount(imageCount), switchTime(switchTime)
{
    uvRect.width = texture->getSize().x / 3;  ///< Largeur d'une frame dans la spritesheet.
    uvRect.height = texture->getSize().y / 4; ///< Hauteur d'une frame dans la spritesheet.
}

/**
 * @brief Met à jour l'animation en fonction du temps écoulé et de l'état du personnage.
 * @param deltaTime Temps écoulé depuis la dernière mise à jour.
 * @param faceRight Indique si le personnage fait face à droite.
 * @param isJumping Indique si le personnage est en train de sauter.
 */
void Animation::update(float deltaTime, bool faceRight, bool isJumping) {
    // Choix de la ligne selon l'état
    int row = isJumping ? (faceRight ? 3 : 2) : (faceRight ? 0 : 1);
    currentImage.y = row;

    // Bloque l'animation si switchTime == 0 (Idle)
    if (switchTime == 0.f) {
        currentImage.x = 0; // On force la première frame de l'animation Idle
    } else {
        totalTime += deltaTime;
        if (totalTime >= switchTime) {
            totalTime -= switchTime;
            currentImage.x++;

            // 3 images par ligne
            if (currentImage.x >= 3) currentImage.x = 0;
        }
    }

    // Découpe la bonne frame dans la spritesheet
    uvRect.left = currentImage.x * uvRect.width;
    uvRect.top = currentImage.y * uvRect.height;
}
