#pragma once

#include "../core/GameConfig.h"
#include "box2d/math_functions.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class World {
private:
    b2WorldId worldId;
    const GameConfig& config;

public:
    World(const GameConfig& config);
    const GameConfig& getConfig() const { return config; }
    b2WorldId getWorldId() const;

    ~World();
};
