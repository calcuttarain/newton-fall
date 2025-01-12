#pragma once

#include <SFML/Graphics.hpp>
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
    bool headless = false;

    // Core systems
    std::unique_ptr<sf::RenderWindow> window;  // Main render window
    sf::Clock clock;         // Game time tracking
    std::unique_ptr<World> world;             // Physics world
    std::unique_ptr<Camera> camera;           // View control

    // Game entities
    std::unique_ptr<Ground> ground;           // Static ground platform
    std::unique_ptr<Square> square;           // Player controlled entity
    std::unique_ptr<Wall> wall;          // Level boundaries
    std::unique_ptr<Background> background;   // Parallax background

    bool gameOver = false;
    sf::Image lastFrame;

    void update();  // Updates game state
    void render();  // Renders frame
    void initializeWindow();
    void cleanupWindow();

public:
    Game(bool headless = false);     // Initializes game systems
    ~Game();
    void loadConfig(const GameConfig& newConfig);
    void run(); // Main game loop
    void restart();
    void step(int action); // 0=nothing, 1=left, 2=right
    bool isGameOver() const { return gameOver; }
    const sf::Image& getLastFrame() const { return lastFrame; }
    void captureFrame();
};
