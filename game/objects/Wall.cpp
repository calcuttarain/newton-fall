#include "Wall.h"
#include "GameConfig.h"
#include "PerlinNoise.h"
#include "RenderableBody.h"
#include "World.h"
#include <box2d/math_functions.h>

Wall::Wall(const World &world, const WallConfig& config) : config(config), perlinNoise((b2Vec2) {config.height, 0.f}, config.nodesCount, config.samplesCount) {
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

    createGraphicsObject(leftWallpoints, leftLineStrip);
    createGraphicsObject(rightWallpoints, rightLineStrip);
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

//todo: sa se coloreze tot ce e de-a stanga zidului din stanga si invers
void Wall::createGraphicsObject(const std::vector<b2Vec2>& points, sf::VertexArray& lineStrip) {
    // Create SFML visual representation using line strip
    lineStrip.setPrimitiveType(sf::LineStrip);
    lineStrip.resize(points.size());
    
    for (size_t i = 0; i < points.size(); i++) {
        lineStrip[i].position = sf::Vector2f(points[i].x, points[i].y);
        lineStrip[i].color = sf::Color::Black;
    }
}

void Wall::render(sf::RenderWindow &window) {
    window.draw(leftLineStrip);
    window.draw(rightLineStrip);
}
