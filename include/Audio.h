#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <iostream>

class AudioManager
{
public:
    AudioManager(); // Constructeur
    void playMenuMusic();
    void playGameMusic();
    void playEndMusic(bool victoire);

private:
    sf::Music musiqueMenu;
    sf::Music musiqueJeu;
    sf::Music musiqueFin;
};

#endif
