#include "Animation.h"

Animation::Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime)
    : imageCount(imageCount), switchTime(switchTime)
{
    uvRect.width = texture->getSize().x / 3;
    uvRect.height = texture->getSize().y / 4;
}

void Animation::update(float deltaTime, bool faceRight, bool isJumping) {
    // Choix de la ligne selon l'état
<<<<<<< HEAD
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
=======
    int row = isJumping ? 3 : (faceRight ? 0 : 1); 
    if (isJumping) {
        row = faceRight ? 3 : 2;  
    } else {
        row = faceRight ? 0 : 1;  
    }
    

    // Met à jour la frame
    currentImage.y = row;
    totalTime += deltaTime;
    if (totalTime >= switchTime) {
        totalTime -= switchTime;
        currentImage.x++;

        // 3 images par ligne
        if (currentImage.x >= 3) currentImage.x = 0;
>>>>>>> b20f099 (mario_animation)
    }

    // Découpe la bonne frame dans la spritesheet
    uvRect.left = currentImage.x * uvRect.width;
    uvRect.top = currentImage.y * uvRect.height;
}
<<<<<<< HEAD

=======
>>>>>>> b20f099 (mario_animation)
