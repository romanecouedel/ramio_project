#include "Audio.h"

AudioManager::AudioManager()
{
    if (!musiqueMenu.openFromFile("../music/Main-Menu.ogg"))
        std::cerr << "Erreur chargement musique menu !" << std::endl;

    if (!musiqueJeu.openFromFile("../music/Main-Title.ogg"))
        std::cerr << "Erreur chargement musique jeu !" << std::endl;

    if (!musiqueFin.openFromFile("../music/mini_gameover.ogg"))
        std::cerr << "Erreur chargement musique fin de jeu !" << std::endl;

    musiqueMenu.setLoop(true);
    musiqueJeu.setLoop(true);
    musiqueFin.setLoop(false);
}

void AudioManager::playMenuMusic()
{
    musiqueJeu.stop();
    musiqueFin.stop();
    musiqueMenu.play();
}

void AudioManager::playGameMusic()
{
    musiqueMenu.stop();
    musiqueFin.stop();
    musiqueJeu.play();
}

void AudioManager::playEndMusic(bool victoire)
{
    musiqueMenu.stop();
    musiqueJeu.stop();
    if (victoire)
        musiqueFin.openFromFile("../music/mini_gameover.ogg");
    else
        musiqueFin.openFromFile("../music/mini_gameover.ogg");
    
    musiqueFin.play();
}
