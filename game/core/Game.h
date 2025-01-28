#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>

#include "GameConfig.h"
#include "Background.h"
#include "Camera.h"
#include "Ground.h"
#include "Square.h"
#include "World.h"
#include "Wall.h"

/**
 * Main Game Class
 * 
 * Centralizes game systems management:
 * - Physics simulation (Box2D)
 * - Graphics rendering (SFML)
 * - Game state and updates
 * - Entity management
 * - Input handling
 */
class Game {
private:
    GameConfig config;      // Central configuration

    // Core systems
    sf::RenderWindow window;  // Main render window
    sf::RenderTexture texture;
    sf::Clock clock;         // Game time tracking
    std::unique_ptr<World> world;             // Physics world
    std::unique_ptr<Camera> camera;           // View control

    // Game entities
    std::unique_ptr<Ground> ground;           // Static ground platform
    std::unique_ptr<Square> square;           // Player controlled entity
    std::unique_ptr<Wall> wall;          // Level boundaries
    std::unique_ptr<Background> background;   // Parallax background

    bool gameOver = false;
    bool instantiate;
    sf::Image lastFrame;
    bool hasWon = false;

    void update();  // Updates game state
    void render();  // Renders frame
    void processGroundCollision();  // New method

public:
    Game(bool instantiate = true);     // Initializes game systems
    void loadConfig(const GameConfig& newConfig);
    void run(); // Main game loop
    void restart();
    void step(int action); // 0=nothing, 1=left, 2=right
    bool isGameOver() const { return gameOver; }
    const sf::Image& getLastFrame() const { return lastFrame; }
    void captureFrame();
    bool isWin() const { return hasWon; }
    float getHealth() const;
    float getPositionY() const;
};
