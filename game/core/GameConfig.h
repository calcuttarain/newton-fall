#pragma once

#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>

/**
 * Game Configuration Structure
 * 
 * Centralized configuration for all game parameters. All measurements are in meters
 * unless otherwise specified. The coordinate system origin is at screen center.
 */
struct GameConfig {
    // Physics configuration
    b2Vec2 gravity{0.0f, -9.8f};  // World gravity vector (m/s^2)

    // Ground parameters
    b2Vec2 groundPosition{0.0f, 0.0f};    // Position relative to world center
    b2Vec2 groundSize{32.0f, 9.0f};       // Width and height in meters

    // Square entity configuration
    b2Vec2 squarePosition{0.0f, 300.0f};  // Initial spawn position
    b2Vec2 squareSize{4.0f, 4.0f};        // Width and height
    float squareDensity{1.0f};            // Mass per unit area
    float squareFriction{0.05f};          // Surface friction coefficient
    float squareBounciness{0.5f};         // Restitution coefficient
    float squareDamageThreshold{10.0f};   // Minimum impact for damage
    float squareMaxHealth{300.0f};        // Starting health points
    float squareInvincibilityTime{1.0f};  // Seconds of invulnerability after hit

    // Display configuration
    sf::Vector2f backgroundSize{800.0f, 10000.0f};  // Background texture size
    sf::Vector2f windowSize{1440.0f, 900.0f};       // Window resolution
    sf::Vector2f viewSize{90.0f, 60.0f};           // Visible game area in meters
    float fps = 120.0f;                            // Target frame rate

    // Wall generation parameters
    float wallHeight = 300.0f;    // Total height of walls
    float wallSpacing = 32.0f;    // Distance between walls
    float amplitude = 6.0f;       // Max sine wave deviation
    float frequency = 0.2f;       // Sine wave frequency
    int pointsCount = 8000;       // Wall resolution (vertex count)
};
