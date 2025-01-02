#pragma once

#include "World.h"
#include "RenderableBody.h"
#include "box2d/math_functions.h"
#include <box2d/box2d.h>

class Ground : public RenderableBody {
private:
  void createShape(b2Vec2 size);
  void createGraphicsObject() override;

public:
  Ground(const World &world, b2Vec2 position, b2Vec2 size);
};
