#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Background {
private:
  sf::RectangleShape visual;  
  sf::Shader shader;

public:
  Background(sf::Vector2f size);

  void render(sf::RenderWindow& window, float u_time);
};

