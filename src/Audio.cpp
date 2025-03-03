#include "Audio.h"

AudioManager audioManager; 

AudioManager::AudioManager()
{
    // Chargement des musiques de fond
    if (!musiqueMenu.openFromFile("../music/Main-Menu.ogg"))
        std::cerr << "Erreur chargement musique menu !" << std::endl;

    if (!musiqueJeu.openFromFile("../music/Main-Title.ogg"))
        std::cerr << "Erreur chargement musique jeu !" << std::endl;

    if (!musiqueFin.openFromFile("../music/win.ogg"))
        std::cerr << "Erreur chargement musique fin de jeu !" << std::endl;

    // Réglage des boucles pour les musiques
    musiqueMenu.setLoop(true);
    musiqueJeu.setLoop(true);
    musiqueFin.setLoop(false);

    // Chargement des effets sonores
    if (!coinBuffer.loadFromFile("../music/Coin.ogg"))
        std::cerr << "Erreur chargement son pièce !" << std::endl;
    else
        coinSound.setBuffer(coinBuffer);

    if (!tuyauBuffer.loadFromFile("../music/tuyau.ogg"))
        std::cerr << "Erreur chargement son tuyau !" << std::endl;
    else
        tuyauSound.setBuffer(tuyauBuffer);

    if (!yahooBuffer.loadFromFile("../music/Yahoo.ogg"))
        std::cerr << "Erreur chargement son Yahoo !" << std::endl;
    else
        yahooSound.setBuffer(yahooBuffer);

    if (!oneUpBuffer.loadFromFile("../music/1UP.ogg"))  
        std::cerr << "Erreur chargement son 1UP !" << std::endl;
    else
        oneUpSound.setBuffer(oneUpBuffer);
}

// Fonctions de lecture des musiques
std::string currentMusic = "";

std::string AudioManager::getCurrentMusic()
{
    return currentMusic;
}

void AudioManager::playMenuMusic()
{
    if (currentMusic == "menu") return;  // Empêche la relance si déjà en cours
    musiqueJeu.stop();
    musiqueFin.stop();
    musiqueMenu.play();
    currentMusic = "menu";
}

void AudioManager::playGameMusic()
{
    if (currentMusic == "game") return;
    musiqueMenu.stop();
    musiqueFin.stop();
    musiqueJeu.play();
    currentMusic = "game";
}

void AudioManager::playEndMusic(bool victoire)
{
    audioManager.setVolume("end", 200.0f);
    musiqueMenu.stop();
    musiqueJeu.stop();
    
    if (victoire)
        musiqueFin.openFromFile("../music/win.ogg");
    else
        musiqueFin.openFromFile("../music/gameover.ogg");

    musiqueFin.play();
    currentMusic = "end";
}


// Fonctions de lecture des effets sonores
void AudioManager::playCoinSound()
{
    audioManager.setVolume("coin", 50.0f);
    coinSound.play();
}

void AudioManager::playTuyauSound()
{
    audioManager.setVolume("tuyau", 50.0f);
    tuyauSound.play();
}

void AudioManager::playYahooSound()
{
    yahooSound.play();
}

void AudioManager::playOneUpSound()
{
    audioManager.setVolume("oneup", 50.0f);
    oneUpSound.play();
}

void AudioManager::setVolume(const std::string& soundName, float volume)
{
    if (soundName == "menu") musiqueMenu.setVolume(volume);
    else if (soundName == "game") musiqueJeu.setVolume(volume);
    else if (soundName == "end") musiqueFin.setVolume(volume);
    else if (soundName == "coin") coinSound.setVolume(volume);
    else if (soundName == "yahoo") yahooSound.setVolume(volume);
    else if (soundName == "tuyau") tuyauSound.setVolume(volume);
    else if (soundName == "oneup") oneUpSound.setVolume(volume);
    else std::cerr << "Son inconnu : " << soundName << std::endl;
}
