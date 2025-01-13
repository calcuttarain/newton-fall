#pragma once

#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>
#include <box2d/math_functions.h>

/**
 * Game Parameters
 *
 * Centralized configuration for all game parameters. All measurements are in
 * meters. The coordinate system origin is at screen center.
 */

struct DisplayConfig{
    sf::Vector2f backgroundSize{800.0f, 10000.0f}; // Background texture size
    sf::Vector2f windowSize{1440.0f, 900.0f};      // Window resolution
    sf::Vector2f viewSize{90.0f, 60.0f};           // Visible game area in meters
    float fps = 120.0f;                            // Target frame rate
};

struct WorldConfig{
    b2Vec2 gravity{0.0f, -9.8f}; // World gravity vector (m/s^2)
};

struct GroundConfig{
    b2Vec2 position{0.0f, 0.0f}; // Position relative to world center
    b2Vec2 size{90.0f, 60.0f};   // Width and height in meters
};

struct SquareConfig{
    b2Vec2 position{0.0f, 1000.0f}; // Initial spawn position
    b2Vec2 size{4.0f, 4.0f};       // Width and height
    float density{1.0f};           // Mass per unit area
    float friction{0.f};         // Surface friction coefficient
    float bounciness{0.5f};        // Restitution coefficient
    float damageThreshold{10.0f};  // Minimum impact for damage
    float maxHealth{200.0f};       // Starting health points
    float invincibilityTime{1.0f}; // Seconds of invulnerability after hit
};

struct WallConfig{
    float height = 1060.0f; 
    float spacing = 40.0f; //distanta dintre zidul stang si cel drept

    //seed diferit pentru fiecare zid
    int leftWallSeed = 1284;
    int rightWallSeed = 1455;

    unsigned int nodesCount = 50; //100 
    unsigned int samplesCount = 20000; //in cate puncte e calculat noise-ul
    unsigned int octavesCount = 3; //detaliile
    float amplitude = 3.f; //amplitudinea primei octave //1.5
    float persistance = 0.6f; //subunitar, cat de repede scade amplitudinea la fiecare octava //0.7
};

struct GameConfig {
    DisplayConfig displayConfig;
    WorldConfig worldConfig;
    GroundConfig groundConfig;
    SquareConfig squareConfig;
    WallConfig wallConfig;
};
