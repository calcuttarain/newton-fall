#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <box2d/box2d.h>  
#include <box2d/id.h>
#include <box2d/math_functions.h>

class RenderableBody {
protected:
  virtual void createGraphicsObject();

  b2BodyId id;            
  b2ShapeId shapeId;
  sf::ConvexShape visual;

public:
    virtual ~RenderableBody() = default;

    b2BodyId getId() const;
    sf::ConvexShape getVisual() const;

    virtual void render(sf::RenderWindow &window);  
};

