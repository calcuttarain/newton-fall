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

    // Core systems
    sf::RenderWindow window;  // Main render window
    sf::Clock clock;         // Game time tracking
    World world;             // Physics world
    Camera camera;           // View control

    // Game entities
    Ground ground;           // Static ground platform
    Square square;           // Player controlled entity
    Wall wall;          // Level boundaries
    Background background;   // Parallax background

    void update();  // Updates game state
    void render();  // Renders frame

public:
    Game();     // Initializes game systems
    void run(); // Main game loop
};
