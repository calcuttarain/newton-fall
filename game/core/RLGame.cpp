#include "RLGame.h"
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
RLGame::RLGame() {
    world = std::make_unique<World>(config.worldConfig);
    ground = std::make_unique<Ground>(*world, config.groundConfig);
    square = std::make_unique<Square>(*world, config.squareConfig);
    wall = std::make_unique<Wall>(*world, config.wallConfig);
    background = std::make_unique<Background>(config.displayConfig.backgroundSize);
    camera = std::make_unique<Camera>(config.displayConfig.viewSize);

    renderTexture.create(config.displayConfig.windowSize.x, config.displayConfig.windowSize.y);
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
void RLGame::run() {
    while (!gameOver) {
        int action = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) action = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) action = 2;

        step(action);
    }
}

void RLGame::render() {
    float u_time = clock.getElapsedTime().asSeconds();
    background->RLrender(renderTexture, u_time);
    ground->RLrender(renderTexture);
    square->RLrender(renderTexture);
    wall->RLrender(renderTexture);
}

void RLGame::update() {
    square->processContactEvents(world->getWorldId());
    float deltaTime = 1.0f / config.displayConfig.fps;
    square->update(deltaTime);
    camera->follow(square->getPosition());
}

void RLGame::loadConfig(const GameConfig& newConfig) {
    config = newConfig;

    renderTexture.create(config.displayConfig.windowSize.x, config.displayConfig.windowSize.y);

    world = std::make_unique<World>(config.worldConfig);
    ground = std::make_unique<Ground>(*world, config.groundConfig);
    square = std::make_unique<Square>(*world, config.squareConfig);
    wall = std::make_unique<Wall>(*world, config.wallConfig);
    background = std::make_unique<Background>(config.displayConfig.backgroundSize);
    camera = std::make_unique<Camera>(config.displayConfig.viewSize);
}

void RLGame::restart() {
    loadConfig(config);
    gameOver = false;
}

void RLGame::step(int action) {
    float timeStep = 1.0f / config.displayConfig.fps;

    switch(action) {
        case 1: // LEFT
            {
                square->handleMovement(true, false);
            }
            break;
        case 2: // RIGHT
            {
                square->handleMovement(false, true);
            }
            break;
    }

    b2World_Step(world->getWorldId(), timeStep, 16);
    update();

    renderTexture.clear();
    render();
    renderTexture.setView(camera->getView());
    renderTexture.display();
    captureFrame();
}

void RLGame::captureFrame() {
    sf::Texture texture = renderTexture.getTexture();
    lastFrame = texture.copyToImage();
}

