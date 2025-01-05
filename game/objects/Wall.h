#pragma once

#include "GameConfig.h"
#include "PerlinNoise.h"
#include "RenderableBody.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>

/**
 * Wall Class
 * 
 * Represents a static chain-based wall in the game world.
 * Combines Box2D chain shape physics with SFML line-based rendering.
 */
class Wall {
private:
    b2BodyId leftWallId;
    b2BodyId rightWallId;

    sf::VertexArray leftLineStrip;  
    sf::VertexArray rightLineStrip;  

    PerlinNoise perlinNoise;

    std::vector<b2Vec2> leftWallpoints;
    std::vector<b2Vec2> rightWallpoints;

    const WallConfig& config;

    /**
     * Creates Box2D chain shape for physics simulation
     */
    void createShape(const b2BodyId& wallId, const std::vector<b2Vec2>& points);

    /**
     * Creates SFML visual representation of the wall
     * Overrides RenderableBody's graphics creation to handle chain shape
     */
    void createGraphicsObject(const std::vector<b2Vec2>& points, sf::VertexArray& lineStrip);

    /**
     * Processes points to reverse their order if needed
     * 
     * @param points Vector of points defining the wall's shape
     * @param reverse If true, reverses point order for proper collision direction
     * @return Processed vector of points
     */
    void generatePerlinNoise();

    void processPoints();

public:
    /**
     * Constructs a wall from a series of points
     * 
     * @param world Reference to the physics world
     * @param points Vector of points defining the wall's shape
     * @param reversePoints If true, reverses point order for proper collision direction
     */
    Wall(const World &world, const WallConfig& config);
    void render(sf::RenderWindow &window); 
};
