#ifndef AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>
#include <iostream>

class AudioManager
{
public:
    AudioManager(); // Constructeur
    std::string getCurrentMusic();
    void setVolume(const std::string& soundName, float volume);
    

    void playMenuMusic();
    void playGameMusic();
    void playEndMusic(bool victoire);
    
    void playCoinSound();
    void playTuyauSound();
    void playYahooSound();
    void playOneUpSound();

private:
    // Musiques de fond
    sf::Music musiqueMenu;
    sf::Music musiqueJeu;
    sf::Music musiqueFin;

    // Effets sonores (buffers et sons)
    sf::SoundBuffer coinBuffer;
    sf::Sound coinSound;

    sf::SoundBuffer tuyauBuffer;
    sf::Sound tuyauSound;

    sf::SoundBuffer yahooBuffer;
    sf::Sound yahooSound;

    sf::SoundBuffer oneUpBuffer;
    sf::Sound oneUpSound;
};

#endif
