#pragma once

#include <SFML/Graphics.hpp>

class Camera {
private:
  float zoomLevel = 1.0f;
  sf::View view;

public:
  Camera(sf::Vector2f viewSize);

  void follow(sf::Vector2f position);
  void setZoom(float zoomLevel);

  sf::View &getView();
};
