#pragma once

#include "../core/GameConfig.h"
#include "World.h"
#include "RenderableBody.h"
#include "box2d/math_functions.h"
#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>
#include <SFML/Graphics/Shader.hpp>

/**
 * Square - A dynamic box-shaped entity that can take damage and display visual feedback
 * 
 * Features:
 * - Physics-based movement using Box2D
 * - Health system with damage tracking
 * - Invincibility frames after taking damage
 * - Visual feedback through shaders based on health state
 */
class Square : public RenderableBody {
private:
    // Core physics and graphics setup
    void createShape(b2Vec2 size, float density, float friction);
    void createGraphicsObject() override;
    
    // Health and damage system
    float health;
    float invincibilityTimer;
    bool isInvincible() const;
    void updateInvincibility(float deltaTime);
    void takeDamage(float amount);
    
    // Collision handling
    void handleCollision(b2BodyId otherBody, const b2Vec2& velocity);
    bool isStaticBody(b2BodyId bodyId) const;
    b2Vec2 previousVelocity;
    
    // Visual feedback system
    sf::Shader shader;
    sf::Clock shaderClock;
    void updateShader();
    
    const GameConfig& config;

public:
    Square();
    Square(const World &world, b2Vec2 position, b2Vec2 size, float density, float friction);

    void update();
    sf::Vector2f getPosition();
    b2Vec2 getVelocity() const;
    void processContactEvents(b2WorldId worldId);
    float getHealth() const { return health; }
    void update(float deltaTime);
    void render(sf::RenderWindow &window) override;
};
