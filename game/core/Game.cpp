#include "Game.h"

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
    float u_time = clock.getElapsedTime().asSeconds();
    if(instantiate)
    {
        background->render(window, u_time);
        ground->render(window);
        square->render(window);
        wall->render(window);
    }
    else 
    {
        background->render(texture, u_time);
        ground->render(texture);
        square->render(texture);
        wall->render(texture);
    }
}

void Game::update() {
    square->processContactEvents(world->getWorldId());
    processGroundCollision();
    
    float deltaTime = 1.0f / config.displayConfig.fps;
    square->update(deltaTime);
    camera->follow(square->getPosition());
    
    // Verificăm dacă jocul s-a terminat 
    if (square->getHealth() <= 0) {
        gameOver = true;
        distanceTraveled = initialY - square->getPosition().y;
        saveStats();
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
    initialY = config.squareConfig.position.y;
    gameTime = 0.0f;
    distanceTraveled = 0.0f;
}

void Game::restart() {
    loadConfig(config);
    gameOver = false;
    hasWon = false;  // Reset win condition
}

void Game::step(int action) {
    float timeStep = 1.0f / config.displayConfig.fps;
    gameTime += timeStep;
    
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
                hasWon = true;
                gameOver = true;
                distanceTraveled = initialY - square->getPosition().y;
                saveStats();
                std::cout << "Level Complete! Health remaining: " << square->getHealth() << std::endl;
            }
        }
    }
}

void Game::saveStats() {
    float hpFinal = square->getHealth();
    float finalDistance = distanceTraveled;
    float totalScore = hpFinal * 2 + finalDistance * 5 - gameTime;
    if (totalScore < 0) totalScore = 0;
    
    std::ofstream file("client/stats.txt", std::ios::trunc); 
    if (file.is_open()) {
        file << finalDistance << "\n"
             << gameTime << "\n"
             << hpFinal << "\n"
             << totalScore;
        file.close();
    } 
}

float Game::getDistanceTraveled() const {
    return initialY - square->getPosition().y;
}

float Game::getHealth() const {
    return square ? square->getHealth() : 0.0f;
}

float Game::getPositionY() const{
    return square ? square->getPosition().y : 0.0f;
}

float Game::getPositionX() const{
    return square ? square->getPosition().x : 0.0f;
}