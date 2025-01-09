#include "Wall.h"
#include "GameConfig.h"
#include "PerlinNoise.h"
#include "RenderableBody.h"
#include "World.h"
#include <box2d/math_functions.h>

Wall::Wall(const World &world, const WallConfig& config) : config(config), perlinNoise((b2Vec2) {config.height, 0.f}, config.nodesCount, config.samplesCount, config.octavesCount, config.amplitude, config.persistance) {
    //box2d wall definition
    b2BodyDef wallDef = b2DefaultBodyDef();
    wallDef.type = b2_staticBody;

    this->leftWallId = b2CreateBody(world.getWorldId(), &wallDef);
    this->rightWallId = b2CreateBody(world.getWorldId(), &wallDef);

    //perlinNoise generator
    leftWallpoints = perlinNoise.generate(config.leftWallSeed);
    rightWallpoints = perlinNoise.generate(config.rightWallSeed);

    processPoints();

    createShape(leftWallId, leftWallpoints);
    createShape(rightWallId, rightWallpoints);

    createGraphicsObject(leftWallpoints, leftWallVisual, true);
    createGraphicsObject(rightWallpoints, rightWallVisual, false);

}

void Wall::processPoints() {
    //punctele din stanga
    for(int i = 0; i < leftWallpoints.size(); i++)
        leftWallpoints[i].x -= config.spacing / 2;

    //punctele din dreapta
    std::reverse(rightWallpoints.begin(), rightWallpoints.end());
    for(int i = 0; i < rightWallpoints.size(); i++)
        rightWallpoints[i].x += config.spacing / 2;
}

void Wall::createShape(const b2BodyId& wallId, const std::vector<b2Vec2>& points)
{
  // Create Box2D chain shape from points
  b2ChainDef chainDef = b2DefaultChainDef();
  chainDef.points = points.data();
  chainDef.count = points.size();

  b2CreateChain(wallId, &chainDef);
}

void Wall::createGraphicsObject(const std::vector<b2Vec2>& points, sf::VertexArray& wallVisual, bool isLeftWall) {
    // folosesc TriangleStrip pentru a construi peretii, TriangleFan avea artefacte
    wallVisual.setPrimitiveType(sf::TriangleStrip);

    if (isLeftWall) {
        for (const auto& point : points) {
            wallVisual.append(sf::Vertex(sf::Vector2f(-100.f, point.y), sf::Color::Black));
            wallVisual.append(sf::Vertex(sf::Vector2f(point.x, point.y), sf::Color::Black)); 
        }
    } else {
        for (const auto& point : points) {
            wallVisual.append(sf::Vertex(sf::Vector2f(point.x, point.y), sf::Color::Black)); 
            wallVisual.append(sf::Vertex(sf::Vector2f(100.f, point.y), sf::Color::Black));  
        }
    }
}

void Wall::render(sf::RenderWindow &window) {
    window.draw(rightWallVisual);
    window.draw(leftWallVisual);
}
