#include "Game.h"
#include <box2d/box2d.h>
#include <iostream>
#include <cmath>
#include <random>

/**
 * Initializes all game systems and entities
 * Sets up:
 * - Physics world with gravity
 * - Window with proper resolution
 * - Ground and walls for level boundaries
 * - Player square with initial position
 * - Camera and background
 */
Game::Game()
    : world(config.worldConfig),  
      ground(world, config.groundConfig),
      square(world, config.squareConfig),
      wall(world, config.wallConfig),  
      background(config.displayConfig.backgroundSize), 
      camera(config.displayConfig.viewSize) {
    window.create(sf::VideoMode(config.displayConfig.windowSize.x, config.displayConfig.windowSize.y),
                 "Newton's Fall");
    window.setFramerateLimit(config.displayConfig.fps);
}

/**
 * Main game loop
 * Handles:
 * - Input processing
 * - Physics simulation steps
 * - Collision processing
 * - State updates
 * - Rendering
 */
void Game::run() {
    float timeStep = 1.0f / config.displayConfig.fps;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                b2Vec2 velocity = b2Body_GetLinearVelocity(square.getId());
                b2Body_SetLinearVelocity(square.getId(), (b2Vec2){-15.0f, velocity.y});
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                b2Vec2 velocity = b2Body_GetLinearVelocity(square.getId());
                b2Body_SetLinearVelocity(square.getId(), (b2Vec2){15.0f, velocity.y});
            }
        }

        b2World_Step(world.getWorldId(), timeStep, 16);

        window.clear();

        update();
        window.setView(camera.getView());
        render();

        window.display();
    }
}

void Game::render() {
  float u_time = this->clock.getElapsedTime().asSeconds();

  background.render(window, u_time);
  ground.render(window);
  square.render(window);
  wall.render(window);
}

void Game::update() {
    square.processContactEvents(world.getWorldId());
    float deltaTime = 1.0f / config.displayConfig.fps;
    square.update(deltaTime);
    camera.follow(square.getPosition());
}
