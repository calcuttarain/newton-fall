#include "Background.h"

Background::Background(sf::Vector2f size) {
  this->visual.setSize(size);
  this->visual.setFillColor(sf::Color::White);
  this->visual.setOrigin(size.x / 2.0f, size.y / 2.0f);
  this->visual.setPosition(0.0f, 0.0f);

  //test shader
  if (!this->shader.loadFromFile("./game/assets/test_background.frag", sf::Shader::Fragment)) {
    std::cerr << "Shader not found!" << std::endl;
  }
}

void Background::render(sf::RenderWindow &window, float u_time) {
  this->shader.setUniform("u_time", u_time);
  window.draw(this->visual, &this->shader);
}
