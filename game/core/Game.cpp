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
Game::Game(bool instantiate) : instantiate(instantiate) {
    // Inițializăm doar config, restul se va face în loadConfig
    // Nu mai creăm obiectele aici
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
    while (window.isOpen() && !gameOver) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        int action = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) action = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) action = 2;
        
        step(action);
    }
}

void Game::render() {
  float u_time = this->clock.getElapsedTime().asSeconds();

  sf::Vector2f squarePosition = square->getPosition();
  squarePosition.x += 600.0f; // Ajustează valoarea până când lumina se aliniază perfect
  squarePosition.y += 10.0f;

  std::cout << "Square Position: " << squarePosition.x << ", " << squarePosition.y << std::endl;



   // Setează uniformele pentru shader-ul fundalului
  background->setLightPosition(squarePosition); // Poziția pătratului
  background->setLightRadius(200.0f);           // Ajustează raza luminii (ex. 200 pixeli)
  background->setLightColor(sf::Vector3f(1.0f, 1.0f, 1.0f)); // Lumină albă

  window.clear();

  background->render(window, u_time);
  ground->render(window);
  square->render(window);
  wall->render(window);

  window.display();

}

void Game::update() {
    square->processContactEvents(world->getWorldId());
    processGroundCollision();  // Add this line
    
    float deltaTime = 1.0f / config.displayConfig.fps;
    square->update(deltaTime);
    camera->follow(square->getPosition());
    
    // Verificăm dacă jocul s-a terminat 
    if (square->getHealth() <= 0) {
        gameOver = true;
        std::cout << "Game Over!" << std::endl;
    }
}

void Game::loadConfig(const GameConfig& newConfig) {
    config = newConfig;
    
    if(instantiate)
    {
        window.create(sf::VideoMode(config.displayConfig.windowSize.x, config.displayConfig.windowSize.y),
                     "Newton's Fall");
        window.setFramerateLimit(config.displayConfig.fps);
    }
    else 
        texture.create(config.displayConfig.windowSize.x, config.displayConfig.windowSize.y);

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
    hasWon = false;  // Reset win condition
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
    if(instantiate)
    {
        window.setView(camera->getView());
        window.clear();
        render();
        window.display();
    }
    else 
    {
        texture.setView(camera->getView());
        texture.clear();
        render();
        texture.display();
        captureFrame();
    }
}

void Game::captureFrame() {
    sf::Texture frame = texture.getTexture();
    lastFrame = frame.copyToImage();
}

void Game::processGroundCollision() {
    b2ContactEvents contactEvents = b2World_GetContactEvents(world->getWorldId());

    for (int i = 0; i < contactEvents.beginCount; ++i) {
        b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;
        
        b2BodyId bodyA = b2Shape_GetBody(beginEvent->shapeIdA);
        b2BodyId bodyB = b2Shape_GetBody(beginEvent->shapeIdB);

        if (b2Body_IsValid(bodyA) && b2Body_IsValid(bodyB)) {
            void* userDataA = b2Body_GetUserData(bodyA);
            void* userDataB = b2Body_GetUserData(bodyB);
            bool isSquareGroundCollision = 
                (userDataA == square.get() && userDataB == ground.get()) ||
                (userDataB == square.get() && userDataA == ground.get());

            if (isSquareGroundCollision) {
                std::cout << "Square-Ground collision detected!" << std::endl;
                if (square->getHealth() > 0) {
                    hasWon = true;
                    gameOver = true;
                    std::cout << "Level Complete! Health remaining: " << square->getHealth() << std::endl;
                }
            }
        }
    }
}

float Game::getHealth() const {
    return square ? square->getHealth() : 0.0f;
}
