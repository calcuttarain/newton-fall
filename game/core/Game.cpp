#include "Game.h"
#include <box2d/box2d.h>
#include <iostream>
#include <cmath>
#include <random>

void Game::initializeWindow() {
    if (!headless) {
        try {
            window = std::make_unique<sf::RenderWindow>(
                sf::VideoMode(
                    static_cast<unsigned int>(config.displayConfig.windowSize.x),
                    static_cast<unsigned int>(config.displayConfig.windowSize.y)
                ),
                "Newton's Fall"
            );
            if (window) {
                window->setFramerateLimit(config.displayConfig.fps);
            }
        } catch (const std::exception& e) {
            std::cerr << "Failed to create window: " << e.what() << std::endl;
            throw;
        }
    }
}

void Game::cleanupWindow() {
    if (window && window->isOpen()) {
        window->close();
    }
    window.reset();
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
Game::Game(bool headless_mode) : headless(headless_mode) {
    try {
        initializeWindow();
        
        world = std::make_unique<World>(config.worldConfig);
        ground = std::make_unique<Ground>(*world, config.groundConfig);
        square = std::make_unique<Square>(*world, config.squareConfig);
        wall = std::make_unique<Wall>(*world, config.wallConfig);
        background = std::make_unique<Background>(config.displayConfig.backgroundSize);
        camera = std::make_unique<Camera>(config.displayConfig.viewSize);
        
    } catch (const std::exception& e) {
        std::cerr << "Game initialization failed: " << e.what() << std::endl;
        cleanupWindow();
        throw;
    }
}

Game::~Game() {
    cleanupWindow();
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
    while (window->isOpen() && !gameOver) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }

        int action = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) action = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) action = 2;
        
        step(action);
    }
}

void Game::render() {
    if (!window) return;
    
    float u_time = clock.getElapsedTime().asSeconds();
    background->render(*window, u_time);
    ground->render(*window);
    square->render(*window);
    wall->render(*window);
}

void Game::update() {
    square->processContactEvents(world->getWorldId());
    float deltaTime = 1.0f / config.displayConfig.fps;
    square->update(deltaTime);
    camera->follow(square->getPosition());
}

void Game::loadConfig(const GameConfig& newConfig) {
    config = newConfig;
    
    initializeWindow();
    
    world = std::make_unique<World>(config.worldConfig);
    ground = std::make_unique<Ground>(*world, config.groundConfig);
    square = std::make_unique<Square>(*world, config.squareConfig);
    wall = std::make_unique<Wall>(*world, config.wallConfig);
    background = std::make_unique<Background>(config.displayConfig.backgroundSize);
    camera = std::make_unique<Camera>(config.displayConfig.viewSize);
}

void Game::restart() {
    loadConfig(config);
    gameOver = false;
}

void Game::step(int action) {
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

    if (!headless && window) {
        window->setView(camera->getView());
        window->clear();
        render();
        window->display();
        captureFrame();
    }
}

void Game::captureFrame() {
    if (!window) return;
    
    sf::Texture texture;
    texture.create(window->getSize().x, window->getSize().y);
    texture.update(*window);
    lastFrame = texture.copyToImage();
}
