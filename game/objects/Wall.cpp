#include "Wall.h"
#include "RenderableBody.h"
#include "World.h"

Wall::Wall(const World &world, std::vector<b2Vec2> points, bool reversePoints) 
    : points(processPoints(points, reversePoints)) {
    b2BodyDef wallDef = b2DefaultBodyDef();
    wallDef.type = b2_staticBody;
    this->id = b2CreateBody(world.getWorldId(), &wallDef);

    createShape(this->points);
    createGraphicsObject();
}

std::vector<b2Vec2> Wall::processPoints(std::vector<b2Vec2> points, bool reverse) {
    if (reverse) {
        std::reverse(points.begin(), points.end());
    }
    return points;
}

void Wall::createShape(std::vector<b2Vec2> points)
{
  // Create Box2D chain shape from points
  b2ChainDef chainDef = b2DefaultChainDef();
  chainDef.points = points.data();
  chainDef.count = points.size();

  this->chainId = b2CreateChain(this->id, &chainDef);
}

void Wall::createGraphicsObject() {
    // Create SFML visual representation using line strip
    lineStrip.setPrimitiveType(sf::LineStrip);
    lineStrip.resize(points.size());
    
    for (size_t i = 0; i < points.size(); i++) {
        lineStrip[i].position = sf::Vector2f(points[i].x, points[i].y);
        lineStrip[i].color = sf::Color::Blue;
    }
}

void Wall::render(sf::RenderWindow &window) {
    window.draw(lineStrip);
}