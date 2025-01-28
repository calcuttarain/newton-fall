#include "Wall.h"
#include "GameConfig.h"
#include "PerlinNoise.h"
#include "RenderableBody.h"
#include "World.h"
#include <box2d/math_functions.h>

Wall::Wall(const World &world, const WallConfig& config) 
    : config(config), 
      perlinNoise((b2Vec2) {config.height, 0.f}, config.nodesCount, config.samplesCount, config.octavesCount, config.amplitude, config.persistance),
      pathNoise((b2Vec2) {config.height, 0.f}, config.pathNodesCount, config.pathSamplesCount, config.pathOctavesCount, config.pathAmplitude, config.pathPersistance) {
    
    b2BodyDef wallDef = b2DefaultBodyDef();
    wallDef.type = b2_staticBody;

    leftWallId = b2CreateBody(world.getWorldId(), &wallDef);
    rightWallId = b2CreateBody(world.getWorldId(), &wallDef);

    // Generate path first
    pathPoints = pathNoise.generate(config.pathSeed);

    // Generate single noise for both walls
    auto wallNoise = perlinNoise.generate(config.leftWallSeed);
    
    // Copy noise for both walls
    leftWallpoints = wallNoise;
    rightWallpoints = wallNoise;

    processPoints();

    createShape(leftWallId, leftWallpoints);
    createShape(rightWallId, rightWallpoints);

    createGraphicsObject(leftWallpoints, leftWallVisual, true);
    createGraphicsObject(rightWallpoints, rightWallVisual, false);
}

void Wall::processPoints() {
    // Păstrăm noise-ul original
    std::vector<b2Vec2> baseNoise = leftWallpoints;

    // Aplicăm path-ul și spacing-ul
    for (size_t i = 0; i < leftWallpoints.size(); i++) {
        float pathOffset = pathPoints[i].x;
        
        // Aplicăm același noise simetric pentru ambele ziduri
        leftWallpoints[i].x = pathOffset - config.spacing / 2.0f + baseNoise[i].x;
        rightWallpoints[i].x = pathOffset + config.spacing / 2.0f + baseNoise[i].x;
        rightWallpoints[i].y = baseNoise[i].y; // Păstrăm aceeași coordonată Y
    }

    // Inversăm punctele zidului din dreapta doar pentru coliziuni corecte
    std::reverse(rightWallpoints.begin(), rightWallpoints.end());
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

void Wall::render(sf::RenderTarget &target) {
    target.draw(rightWallVisual);
    target.draw(leftWallVisual);
}
