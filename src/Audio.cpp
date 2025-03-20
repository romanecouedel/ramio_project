#include "Audio.h"

// Définition de l'instance globale d'AudioManager
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

    if (!musiqueGameOver.openFromFile("../music/gameover.ogg"))
        std::cerr << "Erreur chargement musique Game Over !" << std::endl;

    // Réglage des boucles pour les musiques
    musiqueMenu.setLoop(true);
    musiqueJeu.setLoop(true);
    musiqueFin.setLoop(false);
    musiqueGameOver.setLoop(false);

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

AudioManager::~AudioManager()
{
    // Stopper les musiques avant destruction
    musiqueMenu.stop();
    musiqueJeu.stop();
    musiqueFin.stop();
    musiqueGameOver.stop();

    // Réinitialiser les buffers et sons
    coinSound.resetBuffer();
    tuyauSound.resetBuffer();
    yahooSound.resetBuffer();
    oneUpSound.resetBuffer();
    
    std::cout << "AudioManager détruit" << std::endl;
}


// Retourne la musique en cours
std::string AudioManager::getCurrentMusic()
{
    return currentMusic;
}

// Musique du menu
void AudioManager::playMenuMusic()
{
    if (currentMusic == "menu" || musiqueMenu.getStatus() == sf::Music::Playing) return;
    
    musiqueJeu.stop();
    musiqueFin.stop();
    musiqueGameOver.stop();

    musiqueMenu.play();
    currentMusic = "menu";
}

// Musique du jeu
void AudioManager::playGameMusic()
{
    if (currentMusic == "game" || musiqueJeu.getStatus() == sf::Music::Playing) return;

    musiqueMenu.stop();
    musiqueFin.stop();
    musiqueGameOver.stop();

    musiqueJeu.play();
    currentMusic = "game";
}

// Musique de fin (victoire ou game over)
void AudioManager::playEndMusic(bool victoire)
{
    if (currentMusic == "end") return;

    audioManager.setVolume("end", 200.0f);
    
    musiqueMenu.stop();
    musiqueJeu.stop();
    
    if (victoire)
        musiqueFin.play();
    else
        musiqueGameOver.play();

    currentMusic = "end";
}

// Effets sonores
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

// Gestion du volume
void AudioManager::setVolume(const std::string& soundName, float volume)
{
    if (soundName == "menu") musiqueMenu.setVolume(volume);
    else if (soundName == "game") musiqueJeu.setVolume(volume);
    else if (soundName == "end") {
        musiqueFin.setVolume(volume);
        musiqueGameOver.setVolume(volume);
    }
    else if (soundName == "coin") coinSound.setVolume(volume);
    else if (soundName == "yahoo") yahooSound.setVolume(volume);
    else if (soundName == "tuyau") tuyauSound.setVolume(volume);
    else if (soundName == "oneup") oneUpSound.setVolume(volume);
    else std::cerr << "Son inconnu : " << soundName << std::endl;
}
