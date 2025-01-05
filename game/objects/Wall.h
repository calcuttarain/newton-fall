#pragma once

#include "RenderableBody.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <box2d/id.h>

/**
 * Wall Class
 * 
 * Represents a static chain-based wall in the game world.
 * Combines Box2D chain shape physics with SFML line-based rendering.
 */
class Wall : public RenderableBody {
private:
    b2ChainId chainId;
    std::vector<b2Vec2> points;
    sf::VertexArray lineStrip;  

    /**
     * Creates Box2D chain shape for physics simulation
     */
    void createShape(std::vector<b2Vec2> points);

    /**
     * Creates SFML visual representation of the wall
     * Overrides RenderableBody's graphics creation to handle chain shape
     */
    void createGraphicsObject() override;

    /**
     * Processes points to reverse their order if needed
     * 
     * @param points Vector of points defining the wall's shape
     * @param reverse If true, reverses point order for proper collision direction
     * @return Processed vector of points
     */
    std::vector<b2Vec2> processPoints(std::vector<b2Vec2> points, bool reverse);

public:
    /**
     * Constructs a wall from a series of points
     * 
     * @param world Reference to the physics world
     * @param points Vector of points defining the wall's shape
     * @param reversePoints If true, reverses point order for proper collision direction
     */
    Wall(const World &world, std::vector<b2Vec2> points, bool reversePoints = false);
    void render(sf::RenderWindow &window) override; 
};
