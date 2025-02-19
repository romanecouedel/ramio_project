#include "Animation.h"

Animation::Animation(sf::Texture *texture, sf::Vector2u imageCount, float switchTime)
    : imageCount(imageCount), switchTime(switchTime)
{
    uvRect.width = texture->getSize().x / 3;
    uvRect.height = texture->getSize().y / 4;
}

void Animation::update(float deltaTime, bool faceRight, bool isJumping) {
    // Choix de la ligne selon l'état
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
    }

    // Découpe la bonne frame dans la spritesheet
    uvRect.left = currentImage.x * uvRect.width;
    uvRect.top = currentImage.y * uvRect.height;
}
