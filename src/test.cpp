#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
using namespace sf;

int main() {
  b2WorldDef worldDef = b2DefaultWorldDef();
  worldDef.gravity = (b2Vec2){0.0f, -10.0f};
  b2WorldId worldId = b2CreateWorld(&worldDef);

  b2BodyDef groundBodyDef = b2DefaultBodyDef();
  groundBodyDef.position = (b2Vec2){0.0f, -10.0f};
  b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
  b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
  b2ShapeDef groundShapeDef = b2DefaultShapeDef();
  b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

  b2BodyDef bodyDef = b2DefaultBodyDef();
  bodyDef.type = b2_dynamicBody;
  bodyDef.position = (b2Vec2){0.0f, 4.0f};
  b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
  b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);
  b2ShapeDef shapeDef = b2DefaultShapeDef();
  shapeDef.density = 1.0f;
  shapeDef.friction = 0.3f;
  b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

  RenderWindow window(VideoMode(800, 600), "Test Box2D SFML");
  window.setFramerateLimit(60);

  RectangleShape groundShape(Vector2f(1000.0f, 20.0f));
  groundShape.setOrigin(500.0f, 10.0f);
  groundShape.setPosition(400.0f, 300.0f);
  groundShape.setFillColor(Color::Yellow);

  RectangleShape dynamicShape(Vector2f(20.0f, 20.0f));
  dynamicShape.setOrigin(10.0f, 10.0f);
  dynamicShape.setFillColor(Color::Red);

  float timeStep = 1.0f / 60.0f;
  int subStepCount = 4;

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed)
        window.close();
    }

    b2World_Step(worldId, timeStep, subStepCount);

    b2Vec2 position = b2Body_GetPosition(bodyId);
    b2Rot rotation = b2Body_GetRotation(bodyId);

    dynamicShape.setPosition(400.0f + position.x * 20.0f,
                             300.0f - position.y * 20.0f);
    dynamicShape.setRotation(-b2Rot_GetAngle(rotation) * 180.0f / b2_pi);

    window.clear(Color::Black);
    window.draw(groundShape);
    window.draw(dynamicShape);
    window.display();
  }

  return 0;
}
