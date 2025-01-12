#pragma once

#include "GameConfig.h"
#include "RenderableBody.h"
#include "World.h"
#include "box2d/math_functions.h"
#include <box2d/box2d.h>

class Ground : public RenderableBody {
private:
  const GroundConfig& config;
  void createShape();
  void createGraphicsObject() override;

public:
  Ground(const World &world, const GroundConfig& config);
  void RLrender(sf::RenderTexture &window);
};
