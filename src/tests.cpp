#define BOOST_TEST_MODULE MarioTests
#include <boost/test/included/unit_test.hpp>
#include "Player.h"
#include "Ennemi.h"
#include "Level.h"

// ==================== TESTS PLAYER ====================

BOOST_AUTO_TEST_CASE(test_player_initialization)
{
    Ramio ramio;
    BOOST_CHECK_EQUAL(ramio.isDead, false);
    BOOST_CHECK(ramio.getGlobalBounds().width > 0);
}

BOOST_AUTO_TEST_CASE(test_player_jump)
{
    Ramio ramio;
    float initialY = ramio.getPosition().y;
    
    ramio.jump();
    ramio.update(0.1f, Level()); // Simule une mise à jour après le saut

    BOOST_CHECK(ramio.getPosition().y < initialY); // Maintenant Ramio doit avoir bougé vers le haut
}

// ==================== TESTS ENNEMI ====================

BOOST_AUTO_TEST_CASE(test_ennemi_initialization)
{
    Ennemi goomba;
    BOOST_CHECK(goomba.isAlive);
}

BOOST_AUTO_TEST_CASE(test_ennemi_movement)
{
    Ennemi goomba;
    float initialX = goomba.getPosition().x;

    Level level;
    level.loadFromFile("../levels/level1.txt"); // Charge un vrai niveau

    for (int i = 0; i < 60; i++) // Simule 1 seconde en 60 frames
    {
        goomba.update(0.016f, level);
    }

    std::cout << "Avant: " << initialX << " | Après: " << goomba.getPosition().x << std::endl;

    BOOST_CHECK(goomba.getPosition().x != initialX);
}



// ==================== TESTS LEVEL ====================

BOOST_AUTO_TEST_CASE(test_level_loading)
{
    Level level;
    BOOST_CHECK(level.loadFromFile("../levels/level1.txt"));
}

BOOST_AUTO_TEST_CASE(test_level_collision)
{
    Level level;
    level.loadFromFile("../levels/level1.txt");

    sf::FloatRect testHitbox(50, 50, 64, 64); // Vérifier que cette position est bien libre !
    bool collision = level.isColliding(testHitbox);

    std::cout << "Test collision à (" << testHitbox.left << ", " << testHitbox.top << ") : " 
              << (collision ? "Collision détectée" : "Pas de collision") << std::endl;

    BOOST_CHECK_EQUAL(collision, false);
}
