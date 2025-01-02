#include "Ground.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include <SFML/System/Vector2.hpp>

Ground::Ground(const World &world, b2Vec2 position, b2Vec2 size) {
  b2BodyDef groundBodyDef = b2DefaultBodyDef();

  groundBodyDef.position = position;
  groundBodyDef.type = b2_staticBody;

  this->id = b2CreateBody(world.getWorldId(), &groundBodyDef);
  createShape(size);
  createGraphicsObject();
}

//atribuie shape-ul box2d
void Ground::createShape(b2Vec2 size) {
  b2Polygon groundBox = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
  b2ShapeDef groundShapeDef = b2DefaultShapeDef();
  groundShapeDef.density = 0.0f;
  groundShapeDef.friction = 0.3f;

  this->shapeId = b2CreatePolygonShape(this->id, &groundShapeDef, &groundBox);
}

void Ground::createGraphicsObject()
{
  //se apeleaza metoda care deseneaza forma principala din clasa RenderableBody
  RenderableBody::createGraphicsObject();

  //aici se adauga chestii custom
  this->visual.setFillColor(sf::Color::Black);
}

