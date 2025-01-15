#include "Square.h"
#include "GameConfig.h"
#include "box2d/id.h"
#include "box2d/math_functions.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include <cmath>

/**
 * Initializes a Square with physics properties and visual representation
 * 
 * @param world The physics world this square belongs to
 * @param position Initial position in world coordinates
 * @param size Dimensions of the square
 * @param density Mass per unit area
 * @param friction Coefficient of friction
 */
Square::Square(const World &world, const SquareConfig& config)
    : config(config) {
  b2BodyDef squareDef = b2DefaultBodyDef();

  squareDef.position = config.position;
  squareDef.type = b2_dynamicBody;
  squareDef.userData = this; 

  this->id = b2CreateBody(world.getWorldId(), &squareDef);
  createShape();
  createGraphicsObject();
  previousVelocity = {0.0f, 0.0f}; 
  health = config.maxHealth;
  invincibilityTimer = 0.0f;
  std::cout << "Square health: " << health << std::endl;

  // Încărcăm shaderul cu versiunea GLSL specificată
  if (!shader.loadFromFile("game/assets/square.frag", sf::Shader::Fragment)) {
      std::cerr << "Failed to load square shader!" << std::endl;
  } else {
      shader.setUniform("u_maxHealth", config.maxHealth);
      std::cout << "Square shader loaded successfully!" << std::endl;
  }
}

void Square::createShape() {
  b2Polygon dynamicBox = b2MakeBox(config.size.x / 2.0f, config.size.y / 2.0f);
  b2ShapeDef shapeDef = b2DefaultShapeDef();

  shapeDef.density = config.density;
  shapeDef.friction = config.friction;
  shapeDef.restitution = config.bounciness;  // bouncing

  this->shapeId = b2CreatePolygonShape(this->id, &shapeDef, &dynamicBox);
}

void Square::createGraphicsObject()
{
  //se apeleaza metoda care deseneaza forma principala din clasa RenderableBody
  RenderableBody::createGraphicsObject();

  //aici se adauga chestii custom
  this->visual.setFillColor(sf::Color::White);  // Folosim alb pentru a permite shaderului să controleze culoarea
}

/**
 * Updates physics state and invincibility timer
 * 
 * @param deltaTime Time elapsed since last update
 */
void Square::update(float deltaTime)
{
  sf::Vector2f position = {b2Body_GetPosition(this->id).x, b2Body_GetPosition(this->id).y};
  this->visual.setPosition(position);

  b2Rot rotation = b2Body_GetRotation(this->id);
  float radians = b2Rot_GetAngle(rotation);
  float degrees = radians * 180.0f / b2_pi;
  this->visual.setRotation(degrees);

  // Store previous velocity without printing
  previousVelocity = b2Body_GetLinearVelocity(this->id);

  updateInvincibility(deltaTime);
}

sf::Vector2f Square::getPosition()
{
  sf::Vector2f position = {b2Body_GetPosition(this->id).x, b2Body_GetPosition(this->id).y};
  return position;
}

b2Vec2 Square::getVelocity() const {
  return b2Body_GetLinearVelocity(this->id);
}

bool Square::isStaticBody(b2BodyId bodyId) const {
    return b2Body_GetType(bodyId) == b2_staticBody;
}

bool Square::isInvincible() const {
    return invincibilityTimer > 0.0f;
}

void Square::updateInvincibility(float deltaTime) {
    if (invincibilityTimer > 0.0f) {
        invincibilityTimer = std::max(0.0f, invincibilityTimer - deltaTime);
    }
}

/**
 * Processes damage from collisions and applies invincibility frames
 * 
 * @param amount Amount of damage to apply
 */
void Square::takeDamage(float amount) {
    if (!isInvincible()) {
        float oldHealth = health;
        health = std::max(0.0f, health - amount);
        
        // Only print if health actually changed and set invincibility
        if (oldHealth != health) {
            invincibilityTimer = config.invincibilityTime;  // Mutată aici
            std::cout << "Square health reduced by " << (oldHealth - health) 
                     << "! Current health: " << health << std::endl;
        }
    }
}

void Square::handleCollision(b2BodyId otherBody, const b2Vec2& currentVelocity) {
    b2Vec2 deltaV = {
        previousVelocity.x - currentVelocity.x,
        previousVelocity.y - currentVelocity.y
    };
    
    float impactValue = sqrtf(deltaV.x * deltaV.x + deltaV.y * deltaV.y);
    
    // If impact is significant enough and was moving downward
    if (impactValue > 0.1f && previousVelocity.y < 0) {
        b2Body_SetLinearVelocity(this->id, {currentVelocity.x, 0.0f});
    }
    
    if (impactValue > config.damageThreshold && !isInvincible()) {
        takeDamage(impactValue);
    }
}

void Square::processContactEvents(b2WorldId worldId) {
    b2ContactEvents contactEvents = b2World_GetContactEvents(worldId);

    // Process begin contact events
    for (int i = 0; i < contactEvents.beginCount; ++i) {
        b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;
        
        // Get bodies involved in the collision
        b2BodyId bodyA = b2Shape_GetBody(beginEvent->shapeIdA);
        b2BodyId bodyB = b2Shape_GetBody(beginEvent->shapeIdB);

        // Check if either body is static and valid
        if (b2Body_IsValid(bodyA) && b2Body_IsValid(bodyB)) {
            if (isStaticBody(bodyA) || isStaticBody(bodyB)) {
                // Get the dynamic body
                b2BodyId dynamicBody = isStaticBody(bodyA) ? bodyB : bodyA;
                
                // Compare body IDs using Box2D's API
                if (b2Body_IsValid(dynamicBody) && 
                    b2Body_GetUserData(dynamicBody) == b2Body_GetUserData(this->id)) {
                    handleCollision(dynamicBody, b2Body_GetLinearVelocity(dynamicBody));
                }
            }
        }
    }
}

/**
 * Updates the square's shader uniforms for visual effects
 * Sets health status and time for animations
 */
void Square::updateShader() {
    shader.setUniform("u_health", health);
    shader.setUniform("u_time", shaderClock.getElapsedTime().asSeconds());
}

void Square::render(sf::RenderWindow &window) {
    if (sf::Shader::isAvailable()) {
        updateShader();
        sf::RenderStates states;
        states.shader = &shader;
        window.draw(visual, states);
    } else {
        window.draw(visual);
    }
}



