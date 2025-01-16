#include "RenderableBody.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <cstddef>
#include <iostream>

void RenderableBody::render(sf::RenderTarget &target) {
  target.draw(this->visual);
}

b2BodyId RenderableBody::getId() const { return this->id; }

sf::ConvexShape RenderableBody::getVisual() const { return this->visual; }

// tot ce e scris in functia asta se aplica tuturor obiectelor RenderableBody. Exista
// optiunea de a da override in clasa care o mosteneste la functia asta pentru a
// schimba variabile caracteristice fiecarui obiect (de exemplu, culoare), dar
// trebuie apelata la inceput cea din clasa parinte (am scris un exemplu in
// Ground si Square).
void RenderableBody::createGraphicsObject() {
  // Functioneaza pe orice poligon convex care e
  // admis de Box2D. De citit in
  //  documentatie la sectiunea Shapes despre functia b2Shape_GetPolygon care
  //  returneaza un struct b2Polygon.
  if (b2Shape_GetType(this->shapeId) == b2_polygonShape) {
    size_t numberOfVertices = b2Shape_GetPolygon(this->shapeId).count;
    sf::Vector2f center = {b2Shape_GetPolygon(this->shapeId).centroid.x,
                           b2Shape_GetPolygon(this->shapeId).centroid.y};

    this->visual.setPointCount(numberOfVertices);
    for (int i = 0; i < numberOfVertices; i++) {
      sf::Vector2f point = {b2Shape_GetPolygon(this->shapeId).vertices[i].x,
                            b2Shape_GetPolygon(this->shapeId).vertices[i].y};
      this->visual.setPoint(i, point);
    }
    this->visual.setOrigin(center);
    this->visual.setPosition((sf::Vector2f){b2Body_GetPosition(this->id).x,
                                            b2Body_GetPosition(this->id).y});

    this->visual.setOutlineColor(sf::Color::Black);
    this->visual.setOutlineThickness(0.1f);
  }
  //o sa mai adaug alte tipuri pe parcurs, de exemplu cerc. Chain Shape nu se poate aduce la forma asta generala.
}
