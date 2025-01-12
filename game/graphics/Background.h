#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

class Background {
private:
  sf::RectangleShape visual;  
  sf::Shader shader;

public:
  Background(sf::Vector2f size);

  void render(sf::RenderTarget& target, float u_time);
};

