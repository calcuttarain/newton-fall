#include "Ground.h"
#include "GameConfig.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include <SFML/System/Vector2.hpp>

Ground::Ground(const World &world, const GroundConfig& config) : config(config) {
  b2BodyDef groundBodyDef = b2DefaultBodyDef();

  groundBodyDef.position = config.position;
  groundBodyDef.type = b2_staticBody;
  groundBodyDef.userData = this;  // Adăugăm asta pentru a putea identifica ground-ul în coliziuni

  this->id = b2CreateBody(world.getWorldId(), &groundBodyDef);
  createShape();
  createGraphicsObject();
}

//atribuie shape-ul box2d
void Ground::createShape() {
  b2Polygon groundBox = b2MakeBox(config.size.x / 2.0f, config.size.y / 2.0f);
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
