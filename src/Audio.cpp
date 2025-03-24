#include "Audio.h"

/**
 * @brief Gestionnaire audio global.
 */
AudioManager audioManager; 

/**
 * @brief Constructeur de la classe AudioManager.
 * 
 * Initialise et charge les fichiers audio (musiques et effets sonores).
 */
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

/**
 * @brief Musique actuellement jouée.
 */
std::string currentMusic = "";

/**
 * @brief Récupère la musique actuellement jouée.
 * @return Nom de la musique en cours.
 */
std::string AudioManager::getCurrentMusic()
{
    return currentMusic;
}

/**
 * @brief Joue la musique du menu principal.
 */
void AudioManager::playMenuMusic()
{
    if (currentMusic == "menu") return;  // Empêche la relance si déjà en cours
    musiqueJeu.stop();
    musiqueFin.stop();
    musiqueMenu.play();
    currentMusic = "menu";
}

/**
 * @brief Joue la musique du jeu.
 */
void AudioManager::playGameMusic()
{
    if (currentMusic == "game") return;
    musiqueMenu.stop();
    musiqueFin.stop();
    musiqueJeu.play();
    currentMusic = "game";
}

/**
 * @brief Joue la musique de fin de partie.
 * 
 * @param victoire Indique si le joueur a gagné ou perdu.
 */
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

/**
 * @brief Joue le son d'obtention d'une pièce.
 */
void AudioManager::playCoinSound()
{
    audioManager.setVolume("coin", 50.0f);
    coinSound.play();
}

/**
 * @brief Joue le son d'entrée dans un tuyau.
 */
void AudioManager::playTuyauSound()
{
    audioManager.setVolume("tuyau", 50.0f);
    tuyauSound.play();
}

/**
 * @brief Joue le son "Yahoo!".
 */
void AudioManager::playYahooSound()
{
    yahooSound.play();
}

/**
 * @brief Joue le son d'obtention d'une vie supplémentaire.
 */
void AudioManager::playOneUpSound()
{
    audioManager.setVolume("oneup", 50.0f);
    oneUpSound.play();
}

/**
 * @brief Modifie le volume d'un son ou d'une musique.
 * 
 * @param soundName Nom du son ou de la musique.
 * @param volume Niveau de volume désiré.
 */
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
