#include "Camera.h"
#include <SFML/System/Vector2.hpp>

Camera::Camera(sf::Vector2f viewSize) {
  this->view.setSize(viewSize.x, -viewSize.y);
  this->view.setCenter(0.0f, 0.0f);
}

void Camera::follow(sf::Vector2f position) {
  this->view.setCenter(0.0f, position.y);
}

void Camera::setZoom(float zoom) {
  this->zoomLevel = zoom;
  this->view.zoom(zoom);
}

sf::View &Camera::getView() { return this->view; }
