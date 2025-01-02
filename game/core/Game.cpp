#include "Game.h"
#include <box2d/box2d.h>
#include <iostream>
#include <cmath>
#include <random>

/**
 * Generates procedural wall geometry using sine wave and random noise
 * Creates natural-looking boundaries that provide gameplay variety
 */
std::vector<b2Vec2> Game::generateWallPoints(float xOffset, bool mirror) {
    std::vector<b2Vec2> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> noise(0.0f, 0.2f);

    float heightStep = config.wallHeight / (config.pointsCount - 1);
    
    for (int i = 0; i < config.pointsCount; i++) {
        float y = i * heightStep;
        float x = config.amplitude * std::sin(config.frequency * y);
        x += noise(gen);  // Add noise
        if (mirror) x = -x;
        points.push_back({x + xOffset, y});
    }
    return points;
}

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
    : world(config),  // Updated constructor call
      ground(world, config.groundPosition, config.groundSize),
      square(world, config.squarePosition, config.squareSize,
             config.squareDensity, config.squareFriction),
      leftWall(world, generateWallPoints(-config.wallSpacing/2), false),  // Reverse points for left wall
      rightWall(world, generateWallPoints(config.wallSpacing/2, true), true), // Keep original order for right wall
      background(config.backgroundSize), 
      camera(config.viewSize) {
    window.create(sf::VideoMode(config.windowSize.x, config.windowSize.y),
                 "Newton's Fall");
    window.setFramerateLimit(config.fps);
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
    float timeStep = 1.0f / config.fps;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                b2Vec2 velocity = b2Body_GetLinearVelocity(square.getId());
                b2Body_SetLinearVelocity(square.getId(), (b2Vec2){-5.0f, velocity.y});
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                b2Vec2 velocity = b2Body_GetLinearVelocity(square.getId());
                b2Body_SetLinearVelocity(square.getId(), (b2Vec2){5.0f, velocity.y});
            }
        }

        b2World_Step(world.getWorldId(), timeStep, 8);

        // Process collision events directly in square
        square.processContactEvents(world.getWorldId());

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
  leftWall.render(window);
  rightWall.render(window);
}

void Game::update() {
    float deltaTime = 1.0f / config.fps;
    square.update(deltaTime);
    camera.follow(square.getPosition());
}
