#pragma once

#include "GameConfig.h"
#include "PerlinNoise.h"
#include "RenderableBody.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
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

    sf::VertexArray leftWallVisual;
    sf::VertexArray rightWallVisual;

    PerlinNoise perlinNoise;
    PerlinNoise pathNoise;

    std::vector<b2Vec2> leftWallpoints;
    std::vector<b2Vec2> rightWallpoints;
    std::vector<b2Vec2> pathPoints;

    const WallConfig& config;

    void createShape(const b2BodyId& wallId, const std::vector<b2Vec2>& points);

    void createGraphicsObject(const std::vector<b2Vec2>& points, sf::VertexArray& wallVisual, bool isLeftWall);

    void generatePerlinNoise();
    void generatePath();

    void processPoints();
    void applyPathToWalls();

public:
    Wall(const World &world, const WallConfig& config);

    void render(sf::RenderTarget &target); // Single render method for both window types

};
